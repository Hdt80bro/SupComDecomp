#include "core/NetConnector.h"
#include "gpgcore/MD5.h"
#include "gpgcore/Timer.h"
#include "gpgcore/streams/PipeStream.h"
#include "gpgcore/streams/ZLibStream.h"
#include "boost/thread.hpp"
#include <deque>
#include <vector>

namespace Moho {

static const char *sPacketStateString[] ; // 0x00DFFB68
static const char *sUDPStateStrings[]; // 0x00DFFB8C
    
enum EPacketState : char
{
    CONNECT = 0,
    ANSWER = 1,
    RESETSERIAL = 2,
    SERIALRESET = 3,
    DATA = 4,
    ACK = 5,
    KEEPALIVE = 6,
    GOODBYE = 7,
    NATTRAVERSAL = 8,
    NumStates = 9,
};

struct Moho::SPacket;

struct SPacketHeader
{
    Moho::TDatList<Moho::SPacket, void> mList;
    __int64 mSentTime;
    int mResendCount;
    int mSize;
};

struct /*__unaligned*/ SPacketData
{
    Moho::EPacketState mState;
    int mEarlyMask;
    unsigned __int16 mSerialNumber;
    unsigned __int16 mInResponseTo;
    unsigned __int16 mSequenceNumber;
    unsigned __int16 mExpectedSequenceNumber;
    unsigned __int16 mPayloadLength;
    int mProtocol;
    __int64 mTime;
    Moho::ENetCompressionMethod mCompMethod : 8;
    char mDat1[32];
    char mDat2[32];
    char gap[420]; // until size is 0x200
};



struct SPacket : Moho::SPacketHeader, Moho::SPacketData
{
    std::string ToString(); // 0x00488BC0
    Moho::SPacketData *Data() {
        return static_cast<Moho::SPacketData *>(this);
    }
};

struct struct_ReceivePacket
{
    Moho::SPacket *mPacket;
    u_long mAddr;
    u_short mPort;
};

struct struct_PacketTime
{
    unsigned short mSource;
    gpg::time::Timer mTime;
};

// likely related to `struct_RollingFloat`
struct struct_a1
{
    struct_a2 mDat[4096];
    int mStart;
    int mEnd;

    void Add(int, LONGLONG time, int); // 0x0047D0A0
    struct_a3 GetBetween(LONGLONG start, LONGLONG end); // 0x0047D110
    void Append(struct_a2 *obj); // 0x0047D630
    struct_a2 &Get(size_t ind) {
        return this->mDat[(this->mStart + ind) % 4096];
    }
};


class CNetUDPConnector :
    public Moho::INetConnector,          // 0x00E060D4
    public Moho::INetNATTraversalHandler // 0x00E0610C
{
public:
    boost::recursive_mutex mLock;
    SOCKET mSocket;
    HANDLE mEventObject;
    boost::weak_ptr<Moho::INetNATTraversalHandler *> mNatTravProv;
    Moho::TDatList<Moho::CNetUDPConnection, void> mConnections;
    Moho::TDatList<Moho::SPacket, void> mPacketList;
    int mPacketPoolSize;
    __int64 mInitTime;
    gpg::time::Timer mTimer;
    __int64 mCurTime;
    bool mClosed;
    bool mIsPulling;
    std::deque<struct_ReceivePacket*> mPackets1;
    std::deque<struct_ReceivePacket*> mPackets2;
    HANDLE mSelectedEvent;
    struct_a1 mBuff;
    FILE *mFile;
    int gap;
    
    ~CNetUDPConnector() override; // 0x004899E0
    void Destroy() override; // 0x00489D20
    Moho::ENetProtocol GetProtocol() override; // 0x00485CA0
    u_short GetLocalPort() override; // 0x0048B250
    Moho::INetConnection *Connect(u_long addr, u_short port) override; // 0x0048B2B0
    bool FindNextAddr(__out u_long &addr, __out u_short &port) override; // 0x0048B410
    Moho::INetConnection *Accept(u_long, u_short) override; // 0x0048B4F0
    void Reject(u_long, u_short) override; // 0x0048B500
    void Pull() override; // 0x0048B5C0
    void Push() override; // 0x0048B7F0
    void SelectEvent(HANDLE) override; // 0x0048B9A0
    void Debug() override; // 0x0048B8E0
    struct_a3 Func3(LONGLONG since) override; // 0x0048B9E0
    void Func1(Moho::CMessage *msg) override; // 0x0048BA80
    void ReceivePacket(u_long addr, u_short port, void *dat, size_t size) override; // 0x0048BAE0
    
    void ReceiveTime(Moho::SPacket *pack); // 0x00488220
    CNetUDPConnector(SOCKET sock, boost::weak_ptr<Moho::INetNATTraversalProvider> &prov); // 0x004896F0
    Moho::SPacket *NewPacket(); // 0x00489E80
    void DisposePacket(Moho::SPacket *packet); // 0x00489ED0
    __int64 GetTime(); // 0x00489F30
    void Entry(boost::shared_ptr<void>); // 0x00489F90
    void ReceiveData(); // 0x0048A280
    void ProcessConnect(Moho::SPacket *pack, u_long addr, u_short port); // 0x0048AA40
    int SendData(); // 0x0048AC40
};

enum ENetCompressionMethod
{
    NETCOMP_None = 0x0,
    NETCOMP_Deflate = 0x1,
    NETCOMP_Count = 0x2
};


// 0x00E06118
class CNetUDPConnection : public Moho::INetConnection
{
public:
    Moho::TDatList<Moho::CNetUDPConnection, void> mConnList;
    Moho::CNetUDPConnector *mConnector;
    u_long mAddr;
    u_short mPort;
    WORD gap1;
    ENetCompressionMethod mCompressionMethod;
    ENetCompressionMethod mCompMet;
    Moho::EConnectionState mState;
    gpg::time::Timer mLastSend;
    __int64 mSendTime;
    __int64 mLastRecv;
    __int64 mLastKeepAlive;
    int mKeepAliveFreq;
    char mDat1[32];
    char mDat2[32];
    int v293;
    __int64 mTime1;
    unsigned __int16 mNextSerialNumber;
    unsigned __int16 mInResponseTo;
    gpg::time::Timer mSendBy;
    unsigned __int16 mNextSequenceNumber;
    unsigned __int16 mRemoteExpectedSequenceNumber;
    unsigned __int16 mExpectedSequenceNumber;
    unsigned __int16 v1;
    gpg::PipeStream mPendingOutputData;
    gpg::ZLibOutputFilterStream *mOutputFilterStream;
    bool mHasWritten;
    int mFlushedOutputData;
    bool mOutputShutdown;
    bool mSentShutdown;
    Moho::TDatList<Moho::SPacket, void> mUnackedPayloads;
    struct_PacketTime mTimings[128];
    struct_RollingFloat<25> mPings;
    float mPingTime;
    Moho::TDatList<Moho::SPacket, void> mEarlyPackets;
    unsigned int mMask;
    gpg::PipeStream mInputBuffer;
    gpg::ZLibOutputFilterStream *mFilterStream;
    bool mReceivedEndOfInput;
    bool mDispatchedEndOfInput;
    Moho::CMessage mMessage;
    DWORD v866;
    bool mScheduleDestroy;
    bool mDestroyed;
    bool mClosed;
    __int64 mTotalBytesQueued;
    __int64 mTotalBytesSent;
    __int64 mTotalBytesReceived;
    __int64 mTotalBytesDispatched;
    gpg::MD5Context mTotalBytesQueuedMD5;
    gpg::MD5Context mTotalBytesSentMD5;
    gpg::MD5Context mTotalBytesReceivedMD5;
    gpg::MD5Context mTotalBytesDispatchedMD5;
    
    u_long GetAddr() override; // 0x00485BE0
    u_short GetPort() override; // 0x00485BF0
    float GetPing() override; // 0x00489550
    float GetTime() override; // 0x00489590
    void Write(Moho::SDataView *data) override; // 0x00489130
    void Close() override; // 0x004893F0
    std::string ToString() override; // 0x004894C0
    void ScheduleDestroy() override; // 0x00489660
    
    CNetUDPConnection(Moho::CNetUDPConnector *connector, u_long addr, u_short port, Moho::EConnectionState state); // 0x00485D30
    ~CNetUDPConnection(); // 0x00486150
    bool ProcessConnect(Moho::SPacket *pack); // 0x00486380
    void ProcessAnswer(Moho::SPacket *pack); // 0x004865E0
    void CreateFilterStream(); // 0x00486910
    bool ProcessAck(Moho::SPacket *pack); // 0x00486B10
    void ProcessData(Moho::SPacket *pack); // 0x00486DB0
    void ProcessAcknowledge(Moho::SPacket *pack); // 0x00487310
    void ProcessKeepAlive(Moho::SPacket *pack); // 0x00487340
    void ProcessGoodbye(Moho::SPacket *pack); // 0x00487370
    void RecordPacket(Moho::SPacket *pack); // 0x004874C0
    void FlushInput(); // 0x00487590
    void DispatchFromInput(); // 0x004876A0
    bool FlushOutput(); // 0x004879E0
    void Debug(); // 0x00487B90
    __int64 CalcResendDelay(Moho::SPacket *pack); // 0x00488170
    int TimeSince(LONGLONG time); // 0x004881F0
    void ReceiveTime(Moho::SPacket *pack); // 0x00488220
    int GetBacklog(__int64 time); // 0x00488260
    bool GetBacklogTimeout(LONGLONG time, __out int &timeout); // 0x004882C0
    int SendData(); // 0x00488300
    bool HasPacketWaiting(__int64 time); // 0x00488730
    Moho::SPacket *NewConnectionPacket(); // 0x00488810
    Moho::SPacket *NewAnswerPacket(); // 0x004888C0
    Moho::SPacket *ReadPacket(); // 0x00488980
    Moho::SPacket *NewGoodbyePacket(); // 0x00488AA0
    Moho::SPacket *NewPacket(Moho::EPacketState state, int size, bool inherit); // 0x00488B20
    void SendPacket(Moho::SPacket *pack); // 0x00488D80
};

Moho::INetConnector *NET_MakeUDPConnector(u_short, boost::weak_ptr<Moho::INetNATTraversalProvider> *); // 0x0048BBE0

}


std::string func_FileTimeToString(LONGLONG time); // 0x00485CB0
void func_UDPPacketLog(Moho::SPacket *packet, const char *type, LONGLONG time); // 0x00487A30
int func_ChooseTimeout(int old, int choice); // 0x00488150
void func_LogPacket(Moho::CNetUDPConnector *conn, __int64 time, u_long addr, u_short port, Moho::SPacketData *dat, int, int); // 0x0048B040
