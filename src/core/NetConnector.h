#include "core/Message.h"
#include "core/TDatList.h"
#include "gpgcore/containers/fastvector.h"
#include "gpgcore/streams/Stream.h"
#include "boost/weak_ptr.hpp"
#include <winsock.h>
#include <map>
#include <string>

template<int N>
struct struct_RollingFloat
{
    float mVals[N];
    int mStart;
    int mEnd;

    // N = 25, 0x0048BEC0
    void roll(float in) {
        int nextEnd = (this->mEnd + 1) % N;
        if (nextEnd == this->mStart) {
            this->mStart = (this->mStart + 1) % N;
        }
        this->mVals[this->mStart] = in;
        this->mEnd = nextEnd;
    } // inline
    
    // N = 25, 0x0048BF00
    // N = 10, 0x008D4B20
    float median() {
        float arr[10];
        int pos = 0;
        for (int k = this->mStart; k != this->mEnd; k = (k + 1) % N) {
            arr[pos++] = this->mVals[k];
        }
        std::sort(arr, &arr[pos]);
        return arr[pos / 2];
    }

    // N = 25, 0x0048BF60
    float jitter(float center) {
        float arr[10];
        int pos = 0;
        for (int k = this->mStart; k != this->mEnd; k = (k + 1) % N) {
            arr[pos++] = abs(this->mVals[k] - center);
        }
        std::sort(arr, &arr[pos]);
        return arr[pos / 2];
    }
};

struct struct_a2
{
    DWORD v0;
    DWORD v1;
    LONGLONG mTime;
    size_t mSize;
    DWORD v4;
};

struct struct_a3
{
    std::vector<struct_a2> mVec;
    LONGLONG mDuration;
    LONGLONG mEndTime;

    struct_a3(LONGLONG dur, LONGLONG end) :
        mVec{},
        mDuration{dur},
        mEndTime{end}
    {} // inline e.g. 0x0047D1D0
};

namespace Moho {

static bool net_DebugCrash; // 0x010A6380
static bool net_LogPackets; // 0x010A6381
static int net_DebugLevel; // 0x010A6384
static int net_AckDelay; // 0x00F58DE0
static int net_SendDelay; // 0x00F58DE4
static int net_MinResendDelay; // 0x00F58DE8
static int net_MaxResendDelay; // 0x00F58DEC
static int net_MaxSendRate; // 0x00F58DF0
static int net_MaxBacklog; // 0x00F58DF4
static int net_CompressionMethod; // 0x00F58DF8
static float net_ResendPingMultiplier; // 0x00F58DFC
static int new_ResendDelayBias; // 0x00F58E00

enum ENetProtocol
{
    NETPROTO_None = 0x0,
    NETPROTO_TCP = 0x1,
    NETPROTO_UDP = 0x2,
};


enum EConnectionState
{
    Pending = 0,
    Connecting = 1,
    Answering = 2,
    Established = 3,
    TimedOut = 4,
    Errored = 5,
    NumStates = 6,
};

struct SDataView
{
    char *mStart;
    char *mEnd;
};

// 0x00E0499C
class INetConnection : public Moho::CMessageDispatcher
{
public:
    DWORD v259;
    
    virtual u_long GetAddr() = 0;
    virtual u_short GetPort() = 0;
    virtual float GetPing() = 0;
    virtual float GetTime() = 0;
    virtual void Write(Moho::SDataView *data) = 0;
    virtual void Close() = 0;
    virtual std::string ToString() = 0;
    virtual void ScheduleDestroy() = 0;
};

// 0x00E03CB0
class INetConnector
{
public:
    virtual ~INetConnector() = default; // 0x0047EAE0
    virtual void Destroy() = 0;
    virtual Moho::ENetProtocol GetProtocol() = 0;
    virtual u_short GetLocalPort() = 0;
    virtual Moho::INetConnection *Connect(u_long addr, u_short port) = 0;
    virtual bool FindNextAddr(__out u_long &addr, __out u_short &port) = 0;
    virtual Moho::INetConnection *Accept(u_long, u_short) = 0;
    virtual void Reject(u_long, u_short) = 0;
    virtual void Pull() = 0;
    virtual void Push() = 0;
    virtual void SelectEvent(HANDLE) = 0;
    virtual void Debug(); // 0x0047EAD0
    virtual struct_a3 Func3(LONGLONG since) = 0;
};

// 0x00E03CE8
class CNetNullConnector : public Moho::INetConnector
{
public:
    ~CNetNullConnector() override = default; // 0x0047EAE0
    void Destroy() override; // 0x0047EB20
    Moho::ENetProtocol GetProtocol() override; // 0x0047EB30
    u_short GetLocalPort() override; // 0x0047EB40
    Moho::INetConnection *Connect(u_long, u_short) override; // 0x0047EB50
    bool FindNextAddr(u_long &addr, u_short &port) override; // 0x0047EB60
    Moho::INetConnection *Accept(u_long, u_short) override; // 0x0047EB70
    void Reject(u_long, u_short) override; // 0x0047EB80
    void Pull() override; // 0x0047EB90
    void Push() override; // 0x0047EBA0
    void SelectEvent(HANDLE) override; // 0x0047EBA0
    struct_a3 Func3(LONGLONG since) override; // 0x0047EBC0
};

// 0x00E03ED0
class INetDatagramSocket
{
public:
    virtual ~INetDatagramSocket() = default; // 0x0047EF40
    virtual void SendDefault(Moho::CMessage *, u_short) = 0;
    virtual void Send(Moho::CMessage *, u_long addr, u_short port) = 0;
    virtual void Pull() = 0;
    virtual HANDLE CreateEvent() = 0;
};

// 0x00E3EC88
class INetDatagramHandler
{
public:
    virtual void HandleMessage(Moho::CMessage *msg, Moho::INetDatagramSocket *sock, u_long addr, u_short port) = 0;
};

// 0x00E03EE8
class CNetDatagramSocketImpl : public Moho::INetDatagramSocket
{
public:
    Moho::INetDatagramHandler *mDatagramHandler;
    SOCKET mSocket;
    HANDLE mEvent;

    ~CNetDatagramSocketImpl() override; // 0x0047F050
    void SendDefault(Moho::CMessage *, u_short) override; // 0x0047F0D0
    void Send(Moho::CMessage *, u_long addr, u_short port) override; // 0x0047F0F0
    void Pull() override; // 0x0047F190
    HANDLE CreateEvent() override; // 0x0047F330
    
    CNetDatagramSocketImpl(Moho::INetDatagramHandler *handler, SOCKET sock); // inline 0x0047F44E
};

// 0x00E044E4
class INetTCPSocket : public gpg::Stream
{
public:
    SOCKET mSocket;

    virtual u_short GetPort() = 0;
    virtual u_long GetPeerAddr() = 0;
    virtual u_short GetPeerPort() = 0;

    INetTCPSocket() = default; // 0x004827E0
    INetTCPSocket(SOCKET sock) : mSocket{sock} {} // inline
};

// 0x00E0451C
class INetTCPServer
{
public:
    virtual ~INetTCPServer() = default; // 0x00482750
    virtual u_short GetLocalPort() = 0;
    virtual Moho::INetTCPSocket *Accept() = 0;
    virtual void CloseSocket() = 0;

    INetTCPServer() = default; // 0x00482740
};

// 0x00E3D740
class INetNATTraversalProvider
{
public:
    virtual void SetHandler(int port, boost::shared_ptr<Moho::INetNATTraversalHandler> *handler) = 0;
    virtual void ReceivePacket(u_long addr, u_short port, const char *dat, size_t size) = 0;
};

// 0x00E060C8
class INetNATTraversalHandler
{
public:
    virtual void Func1(Moho::CMessage *) = 0;
    virtual void ReceivePacket(u_long addr, u_short port, void *dat, size_t size) = 0;
};

Moho::INetConnector *NET_MakeConnector(u_short, Moho::ENetProtocol, boost::weak_ptr<Moho::INetNATTraversalProvider> *natTravProv); // 0x0047EBF0
std::string NET_GetProtocolName(Moho::ENetProtocol netproto); // 0x0047EC90
Moho::ENetProtocol NET_ProtocolFromString(const char *str); // 0x0047ED50
Moho::INetDatagramSocket *NET_OpenDatagramSocket(unsigned short hostshort, Moho::INetDatagramHandler *datagramHandler); // 0x0047F360
bool NET_Init(); // 0x0047F5A0
const char *NET_GetWinsockErrorString(); // 0x0047F5F0
std::string NET_GetHostName(u_long hostlong); // 0x0047FEE0
bool NET_GetAddrInfo(const char *str, unsigned short unk, bool isTCP, u_long &addr, u_short &port); // 0x0047FF10
std::string NET_GetDottedOctetFromUInt32(unsigned int octets); // 0x004801C0
int NET_GetUInt32FromDottedOcted/*sic*/(std::string octet); // 0x00480200

}

struct struct_Host : Moho::TDatListItem<struct_Host, void>
{
    std::string mName;
    std::map<u_long, struct_Host>::iterator mNode;

    struct_Host(const std::string &name) : // 0x0047F8A0
        Moho::TDatListItem<struct_Host, void>{},
        mName{name},
        mNode{}
    {}
};

struct struct_HostManager
{
    boost::mutex mLock;
    std::map<u_long, struct_Host> mHosts;
    Moho::TDatList<struct_Host, void> mHostList;
};

struct_HostManager *func_GetHostManager(); // 0x0047F990
std::string func_NET_GetHostName(struct_HostManager *, u_long addr); // 0x0047FBE0
