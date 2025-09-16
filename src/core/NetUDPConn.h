#include "core/NetConnector.h"
#include "gpgcore/Timer.h"
#include "gpgcore/streams/PipeStream.h"
#include "gpgcore/streams/ZLibStream.h"
#include <deque>

namespace Moho {
    
enum EPacketState
{
    CONNECT,
    ANSWER,
    RESETSERIAL,
    DATA,
    ACK,
    KEEPALIVE,
    GOODBYE,
    NATTRAVERSAL,
};

struct SPacketHeader
{
    Moho::TDatList<Moho::SPacketHeader, void> mList;
    __int64 mSentTime;
    int mResendCount;
    int mSize;
};

struct __unaligned SPacketData
{
    char mState;
    int mEarlyMask;
    unsigned __int16 mSerialNumber;
    unsigned __int16 mInResponseTo;
    unsigned __int16 mSequenceNumber;
    unsigned __int16 mExpectedSequenceNumber;
    unsigned __int16 mPayloadLength;
    int mVar;
    __int64 mTime;
    char mCompMethod;
    char mDat1[32];
    char mDat2[32];
    char gap[420]; // until size is 0x200
};

struct SPacket : Moho::SPacketHeader, Moho::SPacketData
{
    std::string ToString(); // 0x00488BC0
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

struct struct_a2
{
    DWORD v0;
    DWORD v1;
    FILETIME mTime;
    DWORD mSize;
    DWORD v4;
};

struct struct_a1
{
    struct_a2 mDat[4096];
    int mEnd;
    int mStart;
};


class CNetUDPConnector :
    public Moho::INetConnector,          // 0x00E060D4
    public Moho::INetNATTraversalHandler // 0x00E0610C
{
public:
    enum State
    {
        Pending,
        Connecting,
        Answering,
        Establishing,
        TimedOut,
        Errored,
    };

public:
    struct_SharedLock mLock;
    SOCKET mSocket;
    HANDLE mEventObject;
    boost::weak_ptr<Moho::INetNATTraversalHandler *> mNatTravProv;
    Moho::TDatList<Moho::CNetUDPConnection, void> mConnections;
    Moho::TDatList<Moho::SPacket, void> mPacketList;
    int mPacketPoolSize;
    _FILETIME v14;
    gpg::time::Timer mTimer;
    __int64 mCurTime;
    bool v25a;
    bool v25b;
    std::deque<struct_ReceivePacket> mPackets1;
    std::deque<struct_ReceivePacket> mPackets2;
    struct_a1 mBuff;
    FILE *mFile;
    int gap;
    
    ~CNetUDPConnector() override; // 0x004899E0
    void Destroy() override; // 0x00489D20
    Moho::ENetProtocol GetProtocol() override; // 0x00485CA0
    int GetLocalPort() override; // 0x0048B250
    Moho::INetConnection *Connect(u_long addr, u_short port) override; // 0x0048B2B0
    bool FindNextAddr(__out u_long &addr, __out u_short &port) override; // 0x0048B410
    Moho::INetConnection *Accept(u_long, u_short) override; // 0x0048B4F0
    void Reject(u_long, u_short) override; // 0x0048B500
    void Pull() override; // 0x0048B5C0
    void Push() override; // 0x0048B7F0
    void SelectEvent(HANDLE) override; // 0x0048B9A0
    void Debug() override; // 0x0048B8E0
    void *Func3() override; // 0x0048B9E0
    void Func1(Moho::CMessage *msg) override; // 0x0048BA80
    void ReceivePacket(u_long addr, u_short port, void *dat, size_t size) override; // 0x0048BAE0
    
    CNetUDPConnector(SOCKET sock, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov); // 0x004896F0
    void AddPacket(); // 0x00489ED0
    __int64 GetTime(); // 0x00489F30
    void Entry(boost::shared_ptr<void>); // 0x00489F90
    __int64 ReceiveData(); // 0x0048A280
    void ProcessConnect(Moho::SPacket *pack, u_long addr, u_short port); // 0x0048AA40
};

enum enum_NetCompressionMethod
{
    NETCOMP_None = 0x0,
    NETCOMP_Deflate = 0x1,
};


// 0x00E06118
class CNetUDPConnection : public Moho::INetConnection
{
public:
    enum State
    {

    };

public:
    Moho::CNetUDPConnector *mConnector;
    u_long mAddr;
    u_short mPort;
    WORD gap1;
    enum_NetCompressionMethod compressionMethod;
    int mCompMet;
    Moho::CNetUDPConnection::State mState;
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
    gpg::PipeStream mOutputData;
    gpg::ZLibOutputFilterStream *mOutputFilterStream;
    _BYTE gap500[4];
    DWORD mFlushedOutputData;
    bool mOutputShutdown;
    bool mSentShutdown;
    _WORD gap50E;
    Moho::TDatList<Moho::SPacket, void> mUnackedPayloads;
    struct_PacketTime mTimings[128];
    struct_Speeds mPings;
    float mPingTime;
    Moho::TDatList<Moho::SPacket, void> mEarlyPackets;
    DWORD gapD94;
    gpg::PipeStream mInputBuffer;
    gpg::ZLibOutputFilterStream *mFilterStream;
    bool mReceivedEndOfInput;
    bool mDispatchedEndOfInput;
    Moho::CMessage mMessage;
    DWORD v866;
    bool  mScheduleDestroy;
    bool v867b;
    bool  mClosed;
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
    void Write(struct_DataSpan *data) override; // 0x00489130
    void Close() override; // 0x004893F0
    std::string ToString() override; // 0x004894C0
    void ScheduleDestroy() override; // 0x00489660
    
    CNetUDPConnection(); // 0x00485D30
    ~CNetUDPConnection(); // 0x00486150
    bool ProcessConnect(Moho::SPacket *pack); // 0x00486380
    void ProcessAnswer(Moho::SPacket *pack); // 0x004865E0
    void CreateFilterStream(); // 0x00486910
    bool ProcessAck(Moho::SPacket *pack); // 0x00486B10
    void ProcessData(Moho::SPacket *pack); // 0x00486DB0
    void ProcessKeepAlive(Moho::SPacket *pack); // 0x00487310
    void ProcessGoodbye(Moho::SPacket *pack); // 0x00487340
    void ProcessNATTraversal(Moho::SPacket *pack); // 0x00487370
    __int64 CalcResendDelay(Moho::SPacket *pack); // 0x00488170
    int GetSentTime(__int64 time); // 0x00488260
    int SendData(); // 0x00488300
    bool HasPacketWaiting(__int64 time); // 0x00488730
    Moho::SPacket *NextPacket_0(); // 0x00488810
    Moho::SPacket *NextPacket_1(); // 0x004888C0
    Moho::SPacket *ReadPacket(); // 0x00488980
    Moho::SPacket *NextPacket_7(); // 0x00488AA0
    Moho::SPacket *NextPacket(char state, int size, bool inherit); // 0x00488B20
    void SendPacket(Moho::SPacket *pack); // 0x00488D80
};

Moho::INetConnector *NET_MakeUDPConnector(u_short, boost::weak_ptr<Moho::INetNATTraversalProvider> *); // 0x0048BBE0

}
