#include "NetTCPConn.h"

void *struct_TCPConnLL::GetPtr() {
    struct_TCPConnLL *next = this->next;
    if (next != nullptr) {
        return (void *)(next - 1); // likely a static cast
    } else {
        return nullptr;
    }
}

int Moho::CNetTCPConnection::GetAddr() {
    return this->addr;
}
int Moho::CNetTCPConnection::GetPort() {
    return this->port;
}
float Moho::CNetTCPConnection::GetPing() {
    return 100.0;
}
float Moho::CNetTCPConnection::GetTime() {
    return gpg::time::CyclesToMilliseconds(this->timer1.ElapsedCycles());
}
void Moho::CNetTCPConnection::Write(struct_DataSpan *data) {
    size_t size = data->end - data->start;
    if (size > this->pipestream2.dataEnd - this->pipestream2.writeStart) {
        this->pipestream2.VirtWrite(data->start, size);
    } else {
        memcpy(this->pipestream2.writeStart, data->start, size);
        this->pipestream2.writeStart += size;
    }
}
void Moho::CNetTCPConnection::Close() {
    this->pipestream2.VirtClose(gpg::Stream::ModeSend);
}
std::string Moho::CNetTCPConnection::ToString() {
    return gpg::STR_Printf("%s:%d", Moho::NET_GetHostName(this->addr), this->port);
}
void Moho::CNetTCPConnection::ScheduleDestroy() {
    this->scheduleDestroy = true;
}
Moho::CNetTCPConnection::CNetTCPConnection(Moho::CNetTCPConnector *connector, SOCKET sock, u_long addr, u_short port, int a7) :
    Moho::INetConnection{},
    connector{connector},
    socket{sock},
    v266{a7},
    timer1{},
    pipestream1{},
    pipestream2{},
    size{0},
    hasShutdown{false},
    scheduleDestroy{false}
{
    this->prev = connector->connections.prev;
    this->next = &connector->connections;
    this->prev->prev = this;
    this->prev->next = this;
    if (connector->handle) {
        WSAEventSelect(sock, connector->handle, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}

void Moho::CNetTCPConnection::Push() {
    if (this->scheduleDestroy) {
        delete this;
    } else if (! this->hasShutdown && ! this->inError && (this->v266 == 3 || this->v266 == 4)) {
        while (true) {
            if (this->size < 2048) {
                int avail = this->pipestream2.GetLength();
                if (avail) {
                    int amt = 2048 - this->size;
                    if (amt >= avail ) {
                        amt = avail;
                    }
                    this->pipestream2.Read(&this->buf[this->size], amt);
                    this->size += amt;
                }
            }
            if (! this->size) {
                break;
            }
            int res = send(this->socket, this->buf, this->size, 0);
            if (res == SOCKET_ERROR) {
                if (WSAGetLastError() != 10035) {
                    gpg::Logf("CNetTCPConnection::Push: send() failed: %s", Moho::NET_GetWinsockErrorString());
                    this->inError = true;
                    if (this->connector->handle) {
                        SetEvent(this->connector->handle);
                    }
                }
                return;
            }
            if (res < this->size) {
                memmov(this->buf, &this->buf[res], this->size - res);
            }
            this->size -= res;
        }
        if (this->pipestream2.Empty()) {
            shutdown(this->socket, SD_SEND);
            this->hasShutdown = true;
        }
    }
}

void Moho::CNetTCPConnection::Pull(Moho::TDatListItem<Moho::SPartialConnection> *partConn) {

}


Moho::CNetTCPConnector::CNetTCPConnector(SOCKET sock)
    : Moho::INetConnector{}, socket{sock}
{}
Moho::CNetTCPConnector::~CNetTCPConnector() {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection *conn = cur->Get();
        if (conn != nullptr) {
            delete conn;
        }
    }
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
    }
    // TDatItemList dtrs
    for (struct_TCPConnLL *i = this->ll.next; i; i = this->ll.next) {
        this->ll = i[1];
        i->next = 0;
        i[1].next = 0;
    }
}

void Moho::CNetTCPConnector::Destroy() {
    delete this;
}
Moho::ENetProtocol Moho::CNetTCPConnector::GetProtocol() {
    return Moho::NETPROTO_TCP;
}
int Moho::CNetTCPConnector::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}
Moho::INetConnection *Moho::CNetTCPConnector::Connect(u_long addr, u_short port) {
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): socket() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    int argp = 1;
    if (ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetHostName(addr), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(addr);
    if (connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR && WSAGetLastError() != 10035) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): connect() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    gpg::Logf("CNetTCPConnector::Connect(%s:%d)...", Moho::NET_GetHostName(addr).c_str(), port);
    return new Moho::CNetTCPConnection(this, sock, addr, port, 1);
}
bool Moho::CNetTCPConnector::Func2(OUT u_long &addr, OUT u_short &port) {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection *conn = cur->Get();
        if (conn->v266 != 0) {
            addr = conn->GetAddr();
            port = conn->GetPort();
            return true;
        }
    }
    return false;
}
Moho::INetConnection *Moho::CNetTCPConnector::Accept(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Accept(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection * conn = cur->Get();
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 != 0) {
            conn->v266 = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection(this, INVALID_SOCKET, addr, port, 2);
}
void Moho::CNetTCPConnector::Reject(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Reject(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection * conn = cur->Get();
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 != 0) {
            conn->ScheduleDestroy();
            return;
        }
    }
    gpg::Logf("CNetTCPConnector::Reject(%s:%d): No such connection pending.", Moho::NET_GetHostName(addr).c_str(), port);
}
void Moho::CNetTCPConnector::Pull() {
    sockaddr_in addr;
    int addrlen = sizeof(addr);
    SOCKET sock = accept(this->socket, (SOCKADDR *) &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        if (WSAGetLastError() != 10035) {
            gpg::Logf("CNetTCPConnector::Pull: accept() failed: %s", Moho::NET_GetWinsockErrorString());
        }
    } else {
        u_long host = ntohl(addr.sin_addr.S_un.S_addr);
        u_short port = ntohs(addr.sin_port);
        gpg::Logf("CNetTCPConnector::Pull(): accepted connection from %s:%d", Moho::NET_GetHostName(host).c_str(), port);
        Moho::SPartialConnection * partConn = new Moho::SPartialConnection(sock, host, this, port);
        partConn->prev->next = partConn->next;
        partConn->next->prev = partConn->prev;
        partConn->prev = partConn;
        partConn->next = partConn;
        partConn->prev = this->partialConns.prev;
        partConn->next = &this->partialConns;
        this->partialConns.prev = partConn;
        partConn->prev->next = partConn;
    }

    struct_TCPConnLL *curLL = &this->ll;
    struct_TCPConnLL workingLL;
    workingLL.next = curLL;
    struct_TCPConnLL *startingLL = curLL->next;
    curLL->next = &workingLL;

    if (this->partialConns.next != &this->partialConns) {
        for (auto next = this->partialConns.next; next != &this->partialConns; next = next->next) {
            next->Get()->Pull();
        }
        curLL = workingLL.next;
    }
    for (auto next = this->connections.next; next != &this->connections; next = next->next) {
        Moho::CNetTCPConnection *conn = next->Get();
        conn->Pull(&this->partialConns);
        if (! workingLL.GetPtr()) {
            curLL = workingLL.next;
            break;
        }
    }

    if (curLL) {
        for ( ; curLL->next != &workingLL; curLL = curLL->next + 1)
            ;
        curLL->next = startingLL;
    }
}
void Moho::CNetTCPConnector::Push() {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        cur->Get()->Push();
    }
}
void Moho::CNetTCPConnector::SelectEvent(HANDLE ev) {
    WSAEventSelect(this->socket, ev, FD_ACCEPT);
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        WSAEventSelect(cur->Get()->socket, ev, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}
void *Moho::CNetTCPConnector::Func3() {
    return nullptr; // unknown type;
}

Moho::SPartialConnection::~SPartialConnection() {
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
    }

}

void Moho::SPartialConnection::Pull() {

}


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
int Moho::CNetTCPBuf::Read(char *buf, unsigned int amt, bool isBlocking) {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if (this->failed || ! amt) {
        return 0;
    }
    int avail = this->end - this->readHead;
    int totalRead = 0;
    while (amt > avail) {
        if (avail) {
            memcpy(buf, this->readHead, avail);
            buf += avail;
            amt -= avail;
            totalRead += avail;
        }
        this->readHead = this->buffer;
        this->end = this->buffer;
        while (buf && amt >= 2048) {
            if (! isBlocking) {
                fd_set readfds;
                readfds.fd_array[0] = this->socket;
                readfds.fd_count = 1;
                timeval timeout;
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
                int res = select(1, &readfds, 0, 0, &timeout);
                if (res != SOCKET_ERROR && res <= 0) {
                    return totalRead;
                }
            }
            int res = recv(this->socket, buf, amt, 0);
            if (res == SOCKET_ERROR) {
                gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
                this->failed = 1;
                return totalRead;
            }
            if (res == 0) {
                this->failed = 1;
                return totalRead;
            }
            buf += res;
            amt -= res;
            if (! amt) {
                return totalRead;
            }
        }
        if (! isBlocking && ! this->Select()) {
            return totalRead;
        }
        int res = recv(this->socket, this->buffer, 2048, 0);
        if (res == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
            this->failed = 1;
            return totalRead;
        }
        if (res == 0) {
            this->failed = 1;
            return totalRead;
        }
        this->end = &this->buffer[res];
        avail = this->end - this->readHead;
    }
    memcpy(buf, this->readHead, amt);
    this->readHead += amt;
    return amt + totalRead;
}
void Moho::CNetTCPBuf::Write(const char *buf, unsigned int size) {
    if (size > this->LeftInWriteBuffer()) {
        
        int left = this->LeftInWriteBuffer();
        if (left) {
            memcpy(writeStart, buf, left);
            this->writeStart += left;
            buf += left;
            size -= left;
        }
        this->VirtFlush();
        if (size < 2048) {
            memcpy(&this->buffer[2048], buf, size);
            this->writeStart = &this->buffer[size + 2048];
        } else if (send(this->socket, buf, size, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Write(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }

    } else {
        memcpy(this->writeStart, buf, size);
        this->writeStart += size;
    }
}

BOOL Moho::CNetTCPBuf::Select() {
    fd_set readfds;
    readfds.fd_array[0] = this->socket;
    readfds.fd_count = 1;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int res = select(1, &readfds, 0, 0, &timeout);
    return res == SOCKET_ERROR || res > 0;
}

Moho::CNetTCPBuf::~CNetTCPBuf() {
    this->VirtClose(this->mode);
}

unsigned int Moho::CNetTCPBuf::VirtRead(char *buf, unsigned int len) {
    return this->Read(buf, len, true);
}

unsigned int Moho::CNetTCPBuf::VirtReadNonBlocking(char *buf, unsigned int len) {
    return this->Read(buf, len, false);
}

bool Moho::CNetTCPBuf::VirtAtEnd() {
    if (this->socket = INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    return this->failed;
}

void Moho::CNetTCPBuf::VirtWrite(const char *buf, unsigned int len) {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if ((this->mode & 2) == 0) {
        throw std::runtime_error{std::string{"output shutdown"}};
    }
    if (len) {
        this->Write(buf, len);
    }
}

void Moho::CNetTCPBuf::VirtFlush() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    int len = this->writeStart - this->writeHead;
    if (len)  {
        if (send(socket, &this->buffer[2048], len, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Flush(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        this->writeStart = &this->buffer[2048];
    }
}
void Moho::CNetTCPBuf::VirtClose(gpg::Stream::Mode mode) {
    if (this->socket != INVALID_SOCKET) {
        if ((this->mode & mode & gpg::Stream::ModeSend) != 0) {
            this->VirtFlush();
            shutdown(this->socket, SD_SEND);
            this->mode &=~ gpg::Stream::ModeSend;
            this->dataEnd = 0;
            this->writeStart = 0;
            this->writeHead = 0;
        }
        if ((this->mode & mode & gpg::Stream::ModeReceive) != 0) {
            shutdown(this->socket, SD_RECEIVE);
            this->mode &=~ gpg::Stream::ModeReceive;
            this->end = 0;
            this->readHead = 0;
            this->start = 0;
        }
        if (this->mode != gpg::Stream::ModeNone) {
            closesocket(this->socket);
            this->socket = INVALID_SOCKET;
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


Moho::INetTCPSocket *Moho::NET_TCPConnect(u_long addr, u_short port) {
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
    return new Moho::CNetTCPBuf{sock};
}

Moho::INetTCPServer *Moho::NET_CreateTCPServer(u_long addr, u_short port) {
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

Moho::INetConnector *Moho::NET_MakeTCPConnector(u_short port) {
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
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(0);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: listen() failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPConnector{sock};
}
