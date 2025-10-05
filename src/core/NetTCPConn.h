#include "core/NetConnector.h"
#include "core/TDatList.h"
#include "gpgcore/streams/PipeStream.h"
#include "gpgcore/Timer.h"


struct struct_TCPConnLL
{
    struct_TCPConnLL *mNext;

    void *GetPtr(); // 0x00485830
    ~struct_TCPConnLL(); // inline 0x00484BD5
};

namespace Moho {

class CNetTCPConnector;

// 0x00E049F8
class CNetTCPConnection :
    public Moho::INetConnection,
    public Moho::TDatListItem<Moho::CNetTCPConnection, void>
{
public:
    Moho::CNetTCPConnector *mConnector;
    SOCKET mSocket;
    u_long mAddr;
    u_short mPort;
    int mState;
    int v267;
    gpg::time::Timer mTimer1;
    gpg::PipeStream mPipestream1;
    gpg::PipeStream mPipestream2;
    char mBuff[2048];
    int mSize;
    bool mHasShutdown;
    bool v819b;
    bool v819c;
    bool v819d;
    Moho::CMessage mMessage;
    DWORD v841;
    bool mPushFailed;
    bool mPullFailed;
    bool mScheduleDestroy;
    bool v842d;
    int v843;
    
    u_long GetAddr() override; // 0x004835B0
    u_short GetPort() override; // 0x004835C0
    float GetPing() override; // 0x004835D0
    float GetTime() override; // 0x00484520
    void Write(Moho::SDataView* data) override; // 0x00484540
    void Close() override; // 0x00484590
    std::string ToString() override; // 0x004845B0
    void ScheduleDestroy() override; // 0x004835E0

    CNetTCPConnection(Moho::CNetTCPConnector *connector, SOCKET s, u_long addr, u_short port, int a7); // 0x00483650
    void Push(); // 0x004838D0
    void Pull(Moho::TDatListItem<Moho::SPartialConnection, void> *); // 0x00483A60
    void SelectSocket(HANDLE hndl); // 0x00484640
};

// 0x00E049C0
class CNetTCPConnector :
    public Moho::INetConnector,
    public Moho::INetNATTraversalHandler
{
public:
    struct_TCPConnLL mll;
    SOCKET mSocket;
    Moho::TDatList<Moho::CNetTCPConnection, void> mConnections;
    Moho::TDatList<Moho::SPartialConnection, void> mPartialConns;
    HANDLE mHandle;

    ~CNetTCPConnector() override; // 0x00484AE0
    void Destroy() override; // 0x00483600
    Moho::ENetProtocol GetProtocol() override; // 0x00483610
    u_short GetLocalPort() override; // 0x00484C20
    Moho::INetConnection *Connect(u_long addr, u_short port) override; // 0x00484C50
    bool FindNextAddr(__out u_long &addr, __out u_short &port) override; // 0x00484EA0
    Moho::INetConnection *Accept(u_long, u_short) override; // 0x00484F00
    void Reject(u_long, u_short) override; // 0x00485050
    void Pull() override; // 0x00485190
    void Push() override; // 0x00485610
    void SelectEvent(HANDLE) override; // 0x00485640
    struct_a3 Func3(LONGLONG since) override; // 0x00483620

    CNetTCPConnector(SOCKET sock); // 0x00484AB0
    bool ReadFromStream(SOCKET s, u_long addr, u_short port, gpg::PipeStream *strm); // 0x004853D0
    Moho::CNetTCPConnection *GetConnection(SOCKET s, u_long addr, u_short port); // inline 0x004853F8
};


struct SPartialConnection : Moho::TDatListItem<Moho::SPartialConnection, void>
{
    Moho::CNetTCPConnector *mConnector;
    SOCKET mSocket;
    u_long mAddr;
    u_short mPort;
    gpg::PipeStream mStream;
    Moho::CMessage mmessage;

    ~SPartialConnection(); // 0x004846E0
    SPartialConnection() = default; // 0x00484660
    void Pull(); // 0x00484770
};

// 0x00E04530
class CNetTCPBuf : public Moho::INetTCPSocket
{
public:
    gpg::Stream::Mode mMode;
    bool mFailed;
    char gap;
    char mBuffer[4098];

    ~CNetTCPBuf() override ; // 0x004827C0
    size_t VirtRead(char *buf, size_t len) override; // 0x00482A90
    size_t VirtReadNonBlocking(char * buf, size_t len) override; // 0x00482AB0
    bool VirtAtEnd() override; // 0x00482AD0
    void VirtWrite(const char *buf, size_t len) override; // 0x00482B50
    void VirtFlush() override; // 0x00482CE0
    void VirtClose(gpg::Stream::Mode) override; // 0x00482DA0
    u_short GetPort() override; // 0x00482880
    u_long GetPeerAddr() override; // 0x00482930
    u_short GetPeerPort() override; // 0x004829E0

    CNetTCPBuf(SOCKET sock); // 0x00482770
    int Read(char *buf, size_t len, bool isBlocking); // 0x00482E20
    void Write(const char *buf, size_t len); // inline 0x00482C0A
    BOOL Select(); // 0x00483040        
};

// 0x00E04568
class CNetTCPServerImpl : public Moho::INetTCPServer
{
public:
    SOCKET mSocket;

    ~CNetTCPServerImpl() override; // 0x00483220
    unsigned short GetLocalPort() override; // 0x004832A0
    Moho::INetTCPSocket *Accept() override; // 0x004832D0
    void CloseSocket() override; // 0x00483370

    CNetTCPServerImpl(SOCKET sock); // inline
};

Moho::INetTCPSocket *NET_TCPConnect(u_long addr, u_short port); // 0x004830A0
Moho::INetTCPServer *NET_CreateTCPServer(u_long addr, u_short port); // 0x00483390
Moho::INetConnector *NET_MakeTCPConnector(u_short); // 0x004849A0

}
