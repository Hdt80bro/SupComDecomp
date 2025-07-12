#include "NetConnector.h"
#include "TDatListItem.h"
#include "gpgcore/streams/PipeStream.h"
#include "gpgcore/Timer.h"

struct struct_TCPConnLL
{
    struct_TCPConnLL *next;

    void *GetPtr(); // 0x00485830
};

namespace Moho {

class CNetTCPConnector;

class CNetTCPConnection : public Moho::INetConnection, public Moho::TDatListItem<Moho::CNetTCPConnection>
{
public:
    Moho::CNetTCPConnector *connector;
    SOCKET socket;
    u_long addr;
    u_short port;
    int v266;
    int v267;
    gpg::time::Timer timer1;
    gpg::PipeStream pipestream1;
    gpg::PipeStream pipestream2;
    char buf[2048];
    int size;
    bool hasShutdown;
    bool v819b;
    bool v819c;
    bool v819d;
    struct_Datagram datagram;
    DWORD v841;
    bool inError;
    bool v842b;
    bool scheduleDestroy;
    bool v842d;
    int v843;
    
    
    int GetAddr() override; // 0x004835B0
    int GetPort() override; // 0x004835C0
    float GetPing() override; // 0x004835D0
    float GetTime() override; // 0x00484520
    void Write(struct_DataSpan* data) override; // 0x00484540
    void Close() override; // 0x00484590
    std::string ToString() override; // 0x004845B0
    void ScheduleDestroy() override; // 0x004835E0

    CNetTCPConnection(Moho::CNetTCPConnector *connector, SOCKET s, u_long addr, u_short port, int a7); // 0x00483650
    void Push(); // 0x004838D0
    void Pull(Moho::TDatListItem<Moho::SPartialConnection> *); // 0x00483A60
};

class CNetTCPConnector : public Moho::INetConnector, public Moho::INetNATTraversalHandler
{
public:
    struct_TCPConnLL ll;
    SOCKET socket;
    Moho::TDatListItem<Moho::CNetTCPConnection> connections;
    Moho::TDatListItem<Moho::SPartialConnection> partialConns;
    HANDLE handle;

    ~CNetTCPConnector() override; // 0x00484AE0
    void Destroy() override;
    Moho::ENetProtocol GetProtocol() override;
    int GetLocalPort() override;
    Moho::INetConnection *Connect(u_long addr, u_short port) override;
    bool Func2(OUT u_long &addr, OUT u_short &port) override;
    Moho::INetConnection *Accept(u_long, u_short) override;
    void Reject(u_long, u_short) override;
    void Pull() override; // 0x004838D0
    void Push() override;
    void SelectEvent(HANDLE) override;
    void *Func3() override;

    CNetTCPConnector(SOCKET sock); // 0x00484AB0
};


struct SPartialConnection : Moho::TDatListItem<Moho::SPartialConnection>
{
    Moho::CNetTCPConnector *connector;
    SOCKET socket;
    u_long addr;
    u_short port;
    gpg::PipeStream stream;
    struct_Datagram buf;

    ~SPartialConnection(); // 0x004846E0
    SPartialConnection() = default; // 0x00484660
    void Pull(); // 0x00484770
};


class CNetTCPBuf : public Moho::INetTCPSocket
{
public:
    gpg::Stream::Mode mode;
    bool failed;
    char gap;
    char buffer[4098];

    ~CNetTCPBuf() override ; // 0x004827C0
    unsigned int VirtRead(char * buf, unsigned int len) override; // 0x00482A90
    unsigned int VirtReadNonBlocking(char * buf, unsigned int len) override; // 0x00482AB0
    bool VirtAtEnd() override; // 0x00482AD0
    void VirtWrite(const char *buf, unsigned int len) override; // 0x00482B50
    void VirtFlush() override; // 0x00482CE0
    void VirtClose(gpg::Stream::Mode) override; // 0x00482DA0
    u_short GetPort() override; // 0x00482880
    u_long GetPeerAddr() override; // 0x00482930
    u_short GetPeerPort() override; // 0x004829E0

    CNetTCPBuf() = default; // 0x00482770
    CNetTCPBuf(SOCKET sock); // inline
    int Read(char *buf, unsigned int len, bool isBlocking); // 0x00482E20
    void Write(const char *buf, unsigned int len); // inline 0x00482C0A
    BOOL Select(); // 0x00483040        
};


class CNetTCPServerImpl : public Moho::INetTCPServer
{
public:
    SOCKET socket;

    virtual ~CNetTCPServerImpl(); // 0x00483220
    virtual unsigned short GetLocalPort(); // 0x004832A0
    virtual Moho::INetTCPSocket *Accept(); // 0x004832D0
    virtual void CloseSocket(); // 0x00483370

    CNetTCPServerImpl(SOCKET sock); // inline
};

Moho::INetTCPSocket *NET_TCPConnect(u_long addr, u_short port); // 0x004830A0
Moho::INetTCPServer *NET_CreateTCPServer(u_long addr, u_short port); // 0x00483390

};