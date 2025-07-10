#include "NetConnector.h"

namespace Moho {

    class CNetTCPConnector : public Moho::INetConnector, Moho::INetNATTraversalHandler
    {
    public:
        DWORD v1;
        SOCKET socket;
        DWORD v3;
        DWORD v4;
        DWORD v5;
        DWORD v6;
        HANDLE handle;
    };

    class CNetTCPConnection : public Moho::INetConnection
    {
    public:
        Moho::CNetTCPConnector *connector;
        SOCKET socket;
        int hostlong;
        int hostshort;
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
        gpg::fastvector v820;
        void *v823;
        DWORD v824;
        DWORD v825;
        DWORD v826;
        DWORD v827;
        DWORD v828;
        DWORD v829;
        DWORD v830;
        DWORD v831;
        DWORD v832;
        DWORD v833;
        DWORD v834;
        DWORD v835;
        DWORD v836;
        DWORD v837;
        DWORD v838;
        DWORD v839;
        DWORD v840;
        DWORD v841;
        bool inError;
        bool v842b;
        bool scheduleDestroy;
        bool v842d;
        int v843;
    };

    class CNetTCPBuf : public Moho::INetTCPSocket
    {
    public:
        int v2;
        short v3;
        char buffer[4098];

        ~CNetTCPBuf() override = default;
        unsigned int VirtRead(char * buf, unsigned int len) override;
        unsigned int VirtReadNonBlocking(char * buf, unsigned int len) override;
        bool VirtAtEnd() override;
        void VirtWrite(const char *buf, unsigned int len) override;
        void VirtFlush() override;
        void VirtClose(gpg::Stream::Mode) override;
        u_short GetPort() override;
        u_long GetPeerAddr() override;
        u_short GetPeerPort() override;

        CNetTCPBuf() = default; // 0x00482770
        CNetTCPBuf(SOCKET sock); // inline
        int Read(char *buf, size_t len, bool); // 0x00482E20
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
    
    Moho::INetTCPSocket *NET_TCPConnect(unsigned short port, unsigned int addr); // 0x004830A0
    Moho::INetTCPServer *NET_CreateTCPServer(unsigned int addr, unsigned short port); // 0x00483390
};