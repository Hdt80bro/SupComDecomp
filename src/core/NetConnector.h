#include <string>
#include <winsock.h>
#include "boost/weak_ptr.hpp"
#include "gpgcore/containers/fastvector.h"
#include "gpgcore/streams/Stream.h"
#include "core/Message.h"


struct struct_DataSpan
{
    char *start;
    char *end;
};

namespace Moho {

int net_DebugLevel;

enum ENetProtocol
{
    NETPROTO_None = 0x0,
    NETPROTO_TCP = 0x1,
    NETPROTO_UDP = 0x2,
};

// 0x00E0499C
class INetConnection : public Moho::CMessageDispatcher
{
public:
    DWORD v259;
    
    virtual int GetAddr() = 0;
    virtual int GetPort() = 0;
    virtual float GetPing() = 0;
    virtual float GetTime() = 0;
    virtual void Write(struct_DataSpan *data) = 0;
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
    virtual int GetLocalPort() = 0;
    virtual Moho::INetConnection *Connect(u_long addr, u_short port) = 0;
    virtual bool Func2(OUT u_long &addr, OUT u_short &port) = 0;
    virtual Moho::INetConnection *Accept(u_long, u_short) = 0;
    virtual void Reject(u_long, u_short) = 0;
    virtual void Pull() = 0;
    virtual void Push() = 0;
    virtual void SelectEvent(HANDLE) = 0;
    virtual void *Debug(); // 0x0047EAD0
    virtual void *Func3() = 0;
};

// 0x00E03CE8
class CNetNullConnector : public Moho::INetConnector
{
public:
    ~CNetNullConnector() override = default; // 0x0047EAE0
    void Destroy() override; // 0x0047EB20
    Moho::ENetProtocol GetProtocol() override; // 0x0047EB30
    int GetLocalPort() override; // 0x0047EB40
    Moho::INetConnection *Connect(u_long, u_short) override; // 0x0047EB50
    bool Func2(u_long &addr, u_short &port) override; // 0x0047EB60
    Moho::INetConnection *Accept(u_long, u_short) override; // 0x0047EB70
    void Reject(u_long, u_short) override; // 0x0047EB80
    void Pull() override; // 0x0047EB90
    void Push() override; // 0x0047EBA0
    void SelectEvent(HANDLE) override; // 0x0047EBA0
    void *Func3() override; // 0x0047EBC0
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
    virtual void Pull(Moho::CMessage *buf, Moho::INetDatagramSocket *, u_long, u_short) = 0;
};

// 0x00E03EE8
class CNetDatagramSocketImpl : public Moho::INetDatagramSocket
{
public:
    Moho::INetDatagramHandler *datagramHandler;
    SOCKET socket;
    HANDLE event;

    ~CNetDatagramSocketImpl() override;
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
    SOCKET socket;

    virtual u_short GetPort() = 0;
    virtual u_long GetPeerAddr() = 0;
    virtual u_short GetPeerPort() = 0;

    INetTCPSocket() = default; // 0x004827E0
};

// 0x00E0451C
class INetTCPServer
{
public:
    virtual ~INetTCPServer() = default; // 0x00482750
    virtual unsigned short GetLocalPort() = 0;
    virtual Moho::INetTCPSocket *Accept() = 0;
    virtual void CloseSocket() = 0;

    INetTCPServer() = default; // 0x00482740
};

// 0x00E3D740
class INetNATTraversalProvider
{

};

// 0x00E060C8
class INetNATTraversalHandler
{
public:
};

Moho::INetConnector *NET_MakeConnector(unsigned short, Moho::ENetProtocol, boost::weak_ptr<Moho::INetNATTraversalProvider> *natTravProv); // 0x0047EBF0
std::string NET_GetProtocolName(Moho::ENetProtocol netproto); // 0x0047EC90
Moho::ENetProtocol NET_ProtocolFromString(const char *str); // 0x0047ED50
Moho::INetDatagramSocket *NET_OpenDatagramSocket(unsigned short hostshort, Moho::INetDatagramHandler *datagramHandler); // 0x0047F360
bool NET_Init(); // 0x0047F5A0
const char *NET_GetWinsockErrorString(); // 0x0047F5F0
std::string NET_GetHostName(unsigned long hostlong); // 0x0047FEE0
bool NET_GetAddrInfo(const char *str, unsigned short unk, bool isTCP, unsigned int &addr, unsigned short &port); // 0x0047FF10
std::string NET_GetDottedOctetFromUInt32(unsigned int octets); // 0x004801C0
int Moho::NET_GetUInt32FromDottedOcted/*sic*/(std::string octet); // 0x00480200

Moho::INetConnector *NET_MakeTCPConnector(unsigned short); // 0x004849A0
Moho::INetConnector *NET_MakeUDPConnector(unsigned short, boost::weak_ptr<Moho::INetNATTraversalProvider> *); // 0x0048BBE0

}
