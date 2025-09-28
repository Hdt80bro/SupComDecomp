// implied file

#include "NetTCPConn.h"
#include "gpgcore/General.h"
#include "gpgcore/String.h"
#include "winsock.h"

// 0x00485830
void *struct_TCPConnLL::GetPtr() {
    struct_TCPConnLL *next = this->mNext;
    if (next != nullptr) {
        return (void *)(next - 1); // likely a static cast
    } else {
        return nullptr;
    }
}

// inline 0x00484BD5
struct_TCPConnLL::~struct_TCPConnLL() {
    for (struct_TCPConnLL *i = this->mNext; i; i = this->mNext) {
        this->mNext = i[1].mNext;
        i->mNext = nullptr;
        i[1].mNext = nullptr;
    }
}

// 0x004835B0
u_long Moho::CNetTCPConnection::GetAddr() {
    return this->mAddr;
}

// 0x004835C0
u_short Moho::CNetTCPConnection::GetPort() {
    return this->mPort;
}

// 0x004835D0
float Moho::CNetTCPConnection::GetPing() {
    return 100.0;
}

// 0x00484520
float Moho::CNetTCPConnection::GetTime() {
    return gpg::time::CyclesToMilliseconds(this->mTimer1.ElapsedCycles());
}

// 0x00484540
void Moho::CNetTCPConnection::Write(struct_DataSpan *data) {
    this->mPipestream2.Write(data->mStart, data->mEnd - data->mStart);
}

// 0x00484590
void Moho::CNetTCPConnection::Close() {
    this->mPipestream2.VirtClose(gpg::Stream::ModeWrite);
}

// 0x004845B0
std::string Moho::CNetTCPConnection::ToString() {
    return gpg::STR_Printf("%s:%d", Moho::NET_GetHostName(this->mAddr), this->mPort);
}

// 0x004835E0
void Moho::CNetTCPConnection::ScheduleDestroy() {
    this->mScheduleDestroy = true;
}

// 0x00483650
Moho::CNetTCPConnection::CNetTCPConnection(Moho::CNetTCPConnector *connector, SOCKET sock, u_long addr, u_short port, int state) :
    Moho::INetConnection{},
    Moho::TDatListItem<Moho::CNetTCPConnection, void>{},
    mConnector{connector},
    mSocket{sock},
    mAddr{addr},
    mPort{port},
    mState{state},
    mTimer1{},
    mPipestream1{},
    mPipestream2{},
    mSize{0},
    mHasShutdown{false},
    mMessage{},
    mPushFailed{false},
    mPullFailed{false},
    mScheduleDestroy{false}
{
    this->ListLinkBefore(&connector->mConnections);
    if (connector->mHandle != nullptr) {
        this->SelectSocket(connector->mHandle);
    }
}

// 0x004838D0
void Moho::CNetTCPConnection::Push() {
    if (this->mScheduleDestroy) {
        delete(this);
        return;
    }
    if (this->mHasShutdown || this->mPushFailed || (this->mState != 3 && this->mState != 4)) {
        return;
    }

    if (this->mSize < 2048) {
        int avail = this->mPipestream2.GetLength();
        if (avail != 0) {
            int amt = min(avail, 2048 - this->mSize);
            this->mPipestream2.Read(&this->mBuff[this->mSize], amt);
            this->mSize += amt;
        }
    }
    while (this->mSize != 0) {
        int res = ::send(this->mSocket, this->mBuff, this->mSize, 0);
        if (res == SOCKET_ERROR) {
            if (::WSAGetLastError() != WSAEWOULDBLOCK) {
                gpg::Logf("CNetTCPConnection::Push: send() failed: %s", Moho::NET_GetWinsockErrorString());
                this->mPushFailed = true;
                if (this->mConnector->mHandle != nullptr) {
                    ::SetEvent(this->mConnector->mHandle);
                }
            }
            return;
        }
        if (res < this->mSize) {
            memmov(this->mBuff, &this->mBuff[res], this->mSize - res);
        }
        this->mSize -= res;
        if (this->mSize < 2048) {
            int avail = this->mPipestream2.GetLength();
            if (avail != 0) {
                int amt = min(avail, 2048 - this->mSize);
                this->mPipestream2.Read(&this->mBuff[this->mSize], amt);
                this->mSize += amt;
            }
        }
    }
    if (this->mPipestream2.Empty()) {
        ::shutdown(this->mSocket, SD_SEND);
        this->mHasShutdown = true;
    }
}

// 0x00483A60
void Moho::CNetTCPConnection::Pull(Moho::TDatListItem<Moho::SPartialConnection, void> *partConn) {
    // what ever this list behavior is doing in this function, it's not represented here properly
    struct_TCPConnLL *ll;
    if (this->mConnector != nullptr) {
        ll = &this->mConnector->mll;
    } else {
        ll = nullptr;
    }
    struct_TCPConnLL list;
    struct_TCPConnLL *old;
    if (ll != nullptr) {
        old = ll->mNext;
        ll->mNext = &list;
    } else {
        old = nullptr;
    }

    if (this->mScheduleDestroy) {
        delete(this);
        list.mNext = old;
        return;
    }
    if (this->mState == 1) {
        fd_set writefds;
        writefds.fd_array[0] = this->mSocket;
        writefds.fd_count = 1;
        fd_set exceptfds;
        exceptfds.fd_array[0] = writefds.fd_array[0];
        exceptfds.fd_count = 1;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if (! ::select(0, 0, &writefds, &exceptfds, &timeout)) {
            return;
        }
        if (::_WSAFDIsSet(this->mSocket, &writefds)) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): connection succeeded",
                    Moho::NET_GetHostName(this->mAddr).c_str(), this->mPort);
            this->mState = 3;
            Moho::CMessage msg{0, MSGOP_Msg1};
            this->mPipestream1.Write(msg.mBuff.begin(), msg.mBuff.Size());
        } else if (::_WSAFDIsSet(this->mSocket, &exceptfds)) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): connection failed",
                    Moho::NET_GetHostName(this->mAddr).c_str(), this->mPort);
            this->mState = 5;
            Moho::CMessage msg{0, MSGOP_Msg2};
            auto rec = this->mReceivers[msg.GetType()];
            if (rec != nullptr) {
                rec->Receive(&msg, static_cast<Moho::CMessageDispatcher*>(this));
            };
            if (this->mScheduleDestroy) {
                delete(this);
            }
            return;
        }
        Moho::CMessage msg{0, MSGOP_Advance};
        Moho::CMessageStream strm{&msg};
        u_short port = this->mConnector->GetLocalPort();
        strm.Write((char *) &port, 2);
        this->mPipestream2.Write(strm.mMsg->mBuff);
    } else if (this->mState != 3) {
        if (this->mState == 4 && this->mPushFailed) {
            this->mState = 5;
            Moho::CMessage msg{0, MSGOP_Msg4};
            this->Dispatch(&msg);
        }
        return;
    }
    while (! this->mPullFailed) {
        char buff[2048];
        int v26 = ::recv(this->mSocket, buff, sizeof(buff), 0);
        if (v26 < 0) {
            if (::WSAGetLastError() != WSAEWOULDBLOCK) {
                gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): recv() failed: %s",
                    Moho::NET_GetHostName(this->mAddr).c_str(),
                    this->mPort,
                    Moho::NET_GetWinsockErrorString()
                );
                this->mPipestream1.VirtClose(gpg::Stream::ModeWrite);
                this->mPullFailed = true;
            }
            break;
        }
        this->mTimer1.Reset();
        if (v26) {
            this->mPipestream1.Write(buff, this->mPipestream1.LeftToWrite());
        } else {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): at end of stream.",
                    Moho::NET_GetHostName(this->mAddr).c_str(), this->mPort);
            this->mPipestream1.VirtClose(gpg::Stream::ModeWrite);
            this->mPullFailed = true;
        }
    }
    while (this->mMessage.Read(&this->mPipestream1)) {
        Moho::IMessageReceiver *recr = this->mReceivers[this->mMessage.GetType()];
        if (recr != nullptr) {
            recr->Receive(&this->mMessage, static_cast<Moho::CMessageDispatcher *>(this));
        } else {
            gpg::Logf("No receiver for message type %d received from %s:%d.",
                this->mMessage.GetType(),
                Moho::NET_GetHostName(this->mAddr).c_str(),
                this->mPort
            );
        }
        if (! ebp0) {
            return;
        }
        if (this->mScheduleDestroy) {
            delete(this);
            return;
        }
        this->mMessage.mBuff.Clear();
        this->mMessage.mPos = 0;
    }
    if (unk || this->mPushFailed) {
        this->mState = 5;
        Moho::CMessage msg{0, MSGOP_Msg4};
        Moho::IMessageReceiver *recr = this->mReceivers[msg.GetType()];
        if (recr != nullptr) {
            recr->Receive(&msg, static_cast<Moho::CMessageDispatcher *>(this));
        }
        if (this->mScheduleDestroy) {
            delete(this);
        }
    } else {
        if (! this->mPipestream1.CanRead() || ! this->mPipestream1.VirtAtEnd()) {
            return;
        }
        this->mState = 4;
        Moho::CMessage msg{0, MSGOP_Msg3};
        Moho::IMessageReceiver *recr = this->mReceivers[msg.GetType()];
        if (recr != nullptr) {
            recr->Receive(&msg, static_cast<Moho::CMessageDispatcher *>(this));
        }
        if (this->mScheduleDestroy) {
            delete(this);
        }
    }
}

// 0x00484640
void Moho::CNetTCPConnection::SelectSocket(HANDLE hndl) {
    ::WSAEventSelect(this->mSocket, hndl, FD_READ|FD_CONNECT|FD_CLOSE);
}


// 0x00484AB0
Moho::CNetTCPConnector::CNetTCPConnector(SOCKET sock)
    : Moho::INetConnector{}, mSocket{sock}
{}

// 0x00484AE0
Moho::CNetTCPConnector::~CNetTCPConnector() {
    while (! this->mConnections.ListEmpty()) {
        Moho::CNetTCPConnection *conn = *this->mConnections.begin();
        if (conn != nullptr) {
            delete(conn);
        }
    }
    if (this->mSocket != INVALID_SOCKET) {
        closesocket(this->mSocket);
    }
}

// 0x00483600
void Moho::CNetTCPConnector::Destroy() {
    delete(this);
}

// 0x00483610
Moho::ENetProtocol Moho::CNetTCPConnector::GetProtocol() {
    return Moho::NETPROTO_TCP;
}

// 0x00484C20
u_short Moho::CNetTCPConnector::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getsockname(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}

// 0x00484C50
Moho::INetConnection *Moho::CNetTCPConnector::Connect(u_long addr, u_short port) {
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): socket() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    int argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetHostName(addr), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(addr);
    if (::connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): connect() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    gpg::Logf("CNetTCPConnector::Connect(%s:%d)...", Moho::NET_GetHostName(addr).c_str(), port);
    return new Moho::CNetTCPConnection(this, sock, addr, port, 1);
}

// 0x00484EA0
bool Moho::CNetTCPConnector::FindNextAddr(__out u_long &addr, __out u_short &port) {
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection *conn = *cur;
        if (conn->mState != 0) {
            addr = conn->GetAddr();
            port = conn->GetPort();
            return true;
        }
    }
    return false;
}

// 0x00484F00
Moho::INetConnection *Moho::CNetTCPConnector::Accept(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Accept(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection * conn = *cur;
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->mState != 0) {
            conn->mState = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection(this, INVALID_SOCKET, addr, port, 2);
}

// 0x00485050
void Moho::CNetTCPConnector::Reject(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Reject(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection * conn = *cur;
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->mState != 0) {
            conn->ScheduleDestroy();
            return;
        }
    }
    gpg::Logf("CNetTCPConnector::Reject(%s:%d): No such connection pending.", Moho::NET_GetHostName(addr).c_str(), port);
}

// 0x00485190
void Moho::CNetTCPConnector::Pull() {
    sockaddr_in addr;
    int addrlen = sizeof(addr);
    SOCKET sock = ::accept(this->mSocket, (SOCKADDR *) &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        if (::WSAGetLastError() != WSAEWOULDBLOCK) {
            gpg::Logf("CNetTCPConnector::Pull: accept() failed: %s", Moho::NET_GetWinsockErrorString());
        }
    } else {
        u_long host = ::ntohl(addr.sin_addr.S_un.S_addr);
        u_short port = ::ntohs(addr.sin_port);
        gpg::Logf("CNetTCPConnector::Pull(): accepted connection from %s:%d", Moho::NET_GetHostName(host).c_str(), port);
        auto partConn = new Moho::SPartialConnection{sock, host, this, port};
        partConn->ListLinkBefore(&this->mPartialConns);
    }

    struct_TCPConnLL *curLL = &this->mll;
    struct_TCPConnLL workingLL;
    workingLL.mNext = curLL;
    struct_TCPConnLL *startingLL = curLL->mNext;
    curLL->mNext = &workingLL;

    if (! this->mPartialConns.ListEmpty()) {
        for (auto next = this->mPartialConns.begin(); next != this->mPartialConns.end(); ++next) {
            (*next)->Pull();
        }
        curLL = workingLL.mNext;
    }
    for (auto next = this->mConnections.begin(); next != this->mConnections.end(); ++next) {
        Moho::CNetTCPConnection *conn = *next;
        conn->Pull(&this->mPartialConns);
        if (! workingLL.GetPtr()) {
            curLL = workingLL.mNext;
            break;
        }
    }

    if (curLL) {
        for ( ; curLL->mNext != &workingLL; curLL = curLL->mNext + 1)
            {}
        curLL->mNext = startingLL;
    }
}

// 0x00485610
void Moho::CNetTCPConnector::Push() {
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        (*cur)->Push();
    }
}

// 0x00485640
void Moho::CNetTCPConnector::SelectEvent(HANDLE ev) {
    ::WSAEventSelect(this->mSocket, ev, FD_ACCEPT);
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        ::WSAEventSelect((*cur)->mSocket, ev, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}

// 0x00483620
struct_a3 Moho::CNetTCPConnector::Func3(LONGLONG since) {
    return struct_a3{0, 0};
}

// 0x004853D0
bool Moho::CNetTCPConnector::ReadFromStream(SOCKET s, u_long addr, u_short port, gpg::PipeStream *strm) {
    Moho::CNetTCPConnection *conn = this->GetConnection(s, addr, port);
    Moho::CMessage msg{0, MSGOP_Msg2};
    conn->mPipestream1.Write(msg.mBuff);
    strm->VirtClose(gpg::Stream::ModeSend);
    while (! strm->CanRead() && ! strm->VirtAtEnd()) {
        char buf[2048];
        strm->Read(buf, sizeof(buf));
        conn->mPipestream1.Write(buf, sizeof(buf));
    }
    return true;
}

// inline 0x004853F8
Moho::CNetTCPConnection *Moho::CNetTCPConnector::GetConnection(SOCKET s, u_long addr, u_short port) {
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection * conn = *cur;
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->mState == 2) {
            conn->mSocket = s;
            conn->mState = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection{this, s, addr, port, 0};
}

// 0x004846E0
Moho::SPartialConnection::~SPartialConnection() {
    if (this->mSocket != INVALID_SOCKET) {
        ::closesocket(this->mSocket);
    }
}

// 0x00484770
void Moho::SPartialConnection::Pull() {
    char buf[2048];

    int res = ::recv(this->mSocket, buf, sizeof(buf), 0);
    if (res < 0) {
        delete(this);
        return;
    }
    while (res != 0) {
        this->mStream.Write(buf, res);
        if (this->mBuff.Read(&this->mStream)) {
            Moho::CMessageStream msg{&this->mBuff};
            gpg::BinaryReader v10{&msg};
            v10.Read(&v10.mPort, 2);
            this->mConnector->ReadFromStream(this->mSocket, this->mAddr, v10.mPort, &this->mStream);
            this->mSocket = INVALID_SOCKET;
            delete(this);
            return;
        }
        res = ::recv(this->mSocket, buf, sizeof(buf), 0);
        if (res < 0) {
            if (::WSAGetLastError() == WSAEWOULDBLOCK) {
                return;
            }
            gpg::Logf("SPartialConnection<%s:%d>::Pull(): recv() failed: %s",
                Moho::NET_GetHostName(this->mAddr).c_str(),
                this->mPort,
                Moho::NET_GetWinsockErrorString()
            );
            delete(this);
            return;
        }
    }
    gpg::Logf("SPartialConnection<%s:%d>::Pull(): at end of stream.",
            Moho::NET_GetHostName(this->mAddr).c_str(), this->mPort);
    delete(this);
}


// 0x004827C0
Moho::CNetTCPBuf::~CNetTCPBuf() {
    this->VirtClose(this->mMode);
}

// 0x00482A90
unsigned int Moho::CNetTCPBuf::VirtRead(char *buf, unsigned int len) {
    return this->Read(buf, len, true);
}

// 0x00482AB0
unsigned int Moho::CNetTCPBuf::VirtReadNonBlocking(char *buf, unsigned int len) {
    return this->Read(buf, len, false);
}

// 0x00482AD0
bool Moho::CNetTCPBuf::VirtAtEnd() {
    if (this->mSocket = INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    return this->mFailed;
}

// 0x00482B50
void Moho::CNetTCPBuf::VirtWrite(const char *buf, unsigned int len) {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if ((this->mMode & gpg::Stream::ModeSend) == 0) {
        throw std::runtime_error{std::string{"output shutdown"}};
    }
    if (len != 0) {
        this->Write(buf, len);
    }
}

// 0x00482CE0
void Moho::CNetTCPBuf::VirtFlush() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    int len = this->mWriteStart - this->mWritePtr;
    if (len != 0)  {
        if (::send(socket, &this->mBuffer[2048], len, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Flush(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        this->mWriteStart = &this->mBuffer[2048];
    }
}

// 0x00482DA0
void Moho::CNetTCPBuf::VirtClose(gpg::Stream::Mode mode) {
    if (this->socket != INVALID_SOCKET) {
        if ((this->mMode & mode & gpg::Stream::ModeSend) != 0) {
            this->VirtFlush();
            ::shutdown(this->mSocket, SD_SEND);
            this->mMode &=~ gpg::Stream::ModeSend;
            this->mDataEnd = 0;
            this->mWriteStart = 0;
            this->mWritePtr = 0;
        }
        if ((this->mMode & mode & gpg::Stream::ModeReceive) != 0) {
            ::shutdown(this->mSocket, SD_RECEIVE);
            this->mMode &=~ gpg::Stream::ModeReceive;
            this->mEnd = 0;
            this->mReadPtr = 0;
            this->mStart = 0;
        }
        if (this->mMode != gpg::Stream::ModeNone) {
            ::closesocket(this->mSocket);
            this->mSocket = INVALID_SOCKET;
        }
    }
}

// 0x00482880
u_short Moho::CNetTCPBuf::GetPort() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getsockname(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}

// 0x00482930
u_long Moho::CNetTCPBuf::GetPeerAddr() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getpeername(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohl(name.sin_addr.S_un.S_addr);
}

// 0x004829E0
u_short Moho::CNetTCPBuf::GetPeerPort() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getpeername(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}


// 0x00482770
Moho::CNetTCPBuf::CNetTCPBuf(SOCKET sock) :
    Moho::INetTCPSocket{},
    mSocket{sock}    
{}

// 0x00482E20
int Moho::CNetTCPBuf::Read(char *buf, unsigned int amt, bool isBlocking) {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if (this->mFailed || amt == 0) {
        return 0;
    }
    int avail = this->mEnd - this->mReadPtr;
    int totalRead = 0;
    while (amt > avail) {
        if (avail) {
            memcpy(buf, this->mReadPtr, avail);
            buf += avail;
            amt -= avail;
            totalRead += avail;
        }
        this->mEeadHead = this->mBuffer;
        this->mEnd = this->mBuffer;
        while (buf && amt >= 2048) {
            if (! isBlocking) {
                fd_set readfds;
                readfds.fd_array[0] = this->socket;
                readfds.fd_count = 1;
                timeval timeout;
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
                int res = ::select(1, &readfds, 0, 0, &timeout);
                if (res != SOCKET_ERROR && res <= 0) {
                    return totalRead;
                }
            }
            int res = ::recv(this->mSocket, buf, amt, 0);
            if (res == SOCKET_ERROR) {
                gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
                this->mFailed = true;
                return totalRead;
            }
            if (res == 0) {
                this->mFailed = true;
                return totalRead;
            }
            buf += res;
            amt -= res;
            if (amt == 0) {
                return totalRead;
            }
        }
        if (! isBlocking && ! this->Select()) {
            return totalRead;
        }
        int res = ::recv(this->mSocket, this->mBuffer, 2048, 0);
        if (res == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
            this->mFailed = true;
            return totalRead;
        }
        if (res == 0) {
            this->mFailed = true;
            return totalRead;
        }
        this->mEnd = &this->mBuffer[res];
        avail = this->mEnd - this->mReadPtr;
    }
    memcpy(buf, this->mReadPtr, amt);
    this->mReadPtr += amt;
    return amt + totalRead;
}

// inline 0x00482C0A
void Moho::CNetTCPBuf::Write(const char *buf, unsigned int size) {
    if (size > this->LeftInWriteBuffer()) {
        
        int left = this->LeftInWriteBuffer();
        if (left != 0) {
            memcpy(writeStart, buf, left);
            this->mWriteStart += left;
            buf += left;
            size -= left;
        }
        this->VirtFlush();
        if (size < 2048) {
            memcpy(&this->mBuffer[2048], buf, size);
            this->mWriteStart = &this->mBuffer[size + 2048];
        } else if (send(this->mSocket, buf, size, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Write(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }

    } else {
        memcpy(this->mWriteStart, buf, size);
        this->mWriteStart += size;
    }
}

// 0x00483040     
BOOL Moho::CNetTCPBuf::Select() {
    fd_set readfds;
    readfds.fd_array[0] = this->mSocket;
    readfds.fd_count = 1;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int res = ::select(1, &readfds, 0, 0, &timeout);
    return res == SOCKET_ERROR || res > 0;
}



Moho::CNetTCPServerImpl::CNetTCPServerImpl(SOCKET sock) :
    Moho::INetTCPServer{},
    mSocket{sock}
{}

// 0x00483220
Moho::CNetTCPServerImpl::~CNetTCPServerImpl() {
    if (this->mSocket != INVALID_SOCKET) {
        ::closesocket(this->mSocket);
        this->mSocket = INVALID_SOCKET;
    }
}

// 0x004832A0
unsigned short Moho::CNetTCPServerImpl::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getsockname(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}

// 0x004832D0
Moho::INetTCPSocket *Moho::CNetTCPServerImpl::Accept() {
    SOCKADDR addr;
    int addrlen = sizeof(addr);
    SOCKET sock = ::accept(this->mSocket, &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPServerImpl::Accept(): accept() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}

// 0x00483370
void Moho::CNetTCPServerImpl::CloseSocket() {
    if (this->mSocket != INVALID_SOCKET) {
        ::closesocket(this->mSocket);
        this->mSocket = INVALID_SOCKET;
    }
}


// 0x004830A0
Moho::INetTCPSocket *Moho::NET_TCPConnect(u_long addr, u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_TCPConnect(%s:%d): socket() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(addr);
    if (::connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_TCPConnect(%s:%d): connect() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}

// 0x00483390
Moho::INetTCPServer *Moho::NET_CreateTCPServer(u_long addr, u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_CreateTCPServer(%d): socket() failed: %s", port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(addr);
    if (::bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): bind() failed: %s", port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    if (::listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): listen() failed: %s", port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPServerImpl{sock};
}

// 0x004849A0
Moho::INetConnector *Moho::NET_MakeTCPConnector(u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_MakeConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    unsigned long argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(0);
    if (::bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    if (::listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: listen() failed: %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPConnector{sock};
}
