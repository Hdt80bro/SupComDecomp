#include "NetTCPConn.h"


Moho::CNetTCPServerImpl::CNetTCPServerImpl(SOCKET sock) 
    : Moho::INetTCPServer{}
{
    this->socket = sock;
}
Moho::CNetTCPServerImpl::~CNetTCPServerImpl() {
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
        this->socket = INVALID_SOCKET;
    }
}

Moho::INetTCPSocket *Moho::CNetTCPServerImpl::Accept() {
    SOCKADDR addr;
    int addrlen = sizeof(addr);
    SOCKET sock = accept(this->socket, &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPServerImpl::Accept(): accept() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}

unsigned short Moho::CNetTCPServerImpl::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}

void Moho::CNetTCPServerImpl::CloseSocket() {
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
        this->socket = INVALID_SOCKET;
    }
}

Moho::CNetTCPBuf::CNetTCPBuf(SOCKET sock)
    : Moho::INetTCPSocket{}
{
    this->socket = sock;    
}

int Moho::CNetTCPBuf::Read(char *buf, size_t len, bool a4)
{
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // eax
  size_t v7; // edi
  int v8; // eax
  int v9; // eax
  int v10; // eax
  char *v11; // eax
  const char *WinsockErrorString; // eax
  int v14; // [esp+0h] [ebp-16Ch]
  struct timeval timeout; // [esp+8h] [ebp-164h] BYREF
  std::string v17; // [esp+10h] [ebp-15Ch] BYREF
  std::runtime_error arg1; // [esp+2Ch] [ebp-140h] BYREF
  fd_set readfds; // [esp+54h] [ebp-118h] BYREF
  struct _EXCEPTION_REGISTRATION_RECORD *v20; // [esp+160h] [ebp-Ch]
  void *v21; // [esp+164h] [ebp-8h]
  int v22; // [esp+168h] [ebp-4h]

    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if (this->buffer[0] || ! len) {
        return 0;
    }
    v7 = this->end - this->readHead;
    v14 = 0;
    if ( len <= v7 )
    {
    LABEL_22:
        memcpy(buf, this->readHead, len);
        this->readHead += len;
        return len + v14;
    }
    else
    {
        while ( 1 )
        {
            if ( v7 )
            {
                memcpy(buf, this->readHead, v7);
                buf += v7;
                len -= v7;
                v14 += v7;
            }
            this->readHead = &this->buffer[2];
            this->end = &this->buffer[2];
            if ( buf )
            {
                while ( len >= 0x800 )
                {
                if ( !a4 )
                {
                    readfds.fd_array[0] = this->socket;
                    readfds.fd_count = 1;
                    timeout.tv_sec = 0;
                    timeout.tv_usec = 0;
                    v8 = select(1, &readfds, 0, 0, &timeout);
                    if ( v8 != -1 && v8 <= 0 )
                    return v14;
                }
                v9 = recv(this->socket, buf, len, 0);
                if ( v9 == -1 )
                    goto LABEL_23;
                if ( !v9 )
                    goto LABEL_24;
                buf += v9;
                len -= v9;
                if ( !len )
                    return v14;
                if ( !buf )
                    break;
                }
            }
            if ( !a4 && !sub_483040(this) )
                return v14;
            v10 = recv(this->socket, &this->buffer[2], 2048, 0);
            if ( v10 < 0 )
            {
        LABEL_23:
                gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
        LABEL_24:
                this->buffer[0] = 1;
                return v14;
            }
            if ( !v10 )
                goto LABEL_24;
            v11 = &this->buffer[v10 + 2];
            this->end = v11;
            v7 = v11 - this->readHead;
            if ( len <= v7 )
                goto LABEL_22;
        }
    }
}


u_short Moho::CNetTCPBuf::GetPort() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}

u_long Moho::CNetTCPBuf::GetPeerAddr() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    getpeername(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohl(name.sin_addr.S_un.S_addr);
}

u_short Moho::CNetTCPBuf::GetPeerPort() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    getpeername(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}




Moho::INetTCPSocket *Moho::NET_TCPConnect(unsigned short port, unsigned int addr) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_TCPConnect(%s:%d): socket() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(addr);
    if (connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_TCPConnect(%s:%d): connect() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPBuf(sock);
}

Moho::INetTCPServer *Moho::NET_CreateTCPServer(unsigned int addr, unsigned short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_CreateTCPServer(%d): socket() failed: %s", port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(addr);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): bind() failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): listen() failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPServerImpl{sock};
}

Moho::INetConnector *Moho::NET_MakeTCPConnector(unsigned short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_MakeConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    unsigned long argp = 1;
    if (ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name; // [esp+8h] [ebp-14h] BYREF
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(0);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    } else if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: listen() failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    } else {
        return new Moho::CNetTCPConnector(sock);
    }
}