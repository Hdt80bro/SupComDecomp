// known file

#include "NetUDPConn.h"



// 0x004899E0
Moho::CNetUDPConnector::~CNetUDPConnector() {
    while (this->mPacketList.mNext != &this->mPacketList) {
        if (this->mPacketList.mNext) {
            delete(this->mPacketList.mNext);
        }
    }
    ::closesocket(this->mSocket);
    if (this->mFile != nullptr) { 
        ::fclose(this->mFile);
    }
}

// 0x00489D20
void Moho::CNetUDPConnector::Destroy() {
    boost::shared_ptr<Moho::INetNATTraversalProvider> prov{this->mNatTravProv};
    if (prov) {
        boost::shared_ptr<Moho::INetNATTraversalHandler> nullHandler{};
        int port = this->GetLocalPort();
        prov->SetHandler(port, &nullHandler);
    }
    func_LockShared(&this->mLock);
    for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
        i->ScheduleDestroy();
    }
    this->mClosed = true;
    ::WSASetEvent(this->mEventObject);
    func_UnlockShared(&this->mLock);
}

// 0x00485CA0
Moho::ENetProtocol Moho::CNetUDPConnector::GetProtocol() {
    return Moho::ENetProtocol::NETPROTO_UDP;
}

// 0x0048B250
int Moho::CNetUDPConnector::GetLocalPort() {
    func_LockShared(&this->mLock);
    int namelen = 16;
    sockaddr_in sockaddr;
    ::getsockname(this->mSocket, (SOCKADDR *) &sockaddr, &namelen);
    u_short port = ::ntohs(sockaddr.sin_port);
    func_UnlockShared(&this->mLock);
    return port;
}

// 0x0048B2B0
Moho::INetConnection *Moho::CNetUDPConnector::Connect(u_long addr, u_short port) {
    func_LockShared(&this->mLock);
    ::WSASetEvent(this->mEventObject);
    for (auto itr = this->mConnections.begin(); itr != this->mConnections.end(); ++itr) {
        Moho::CNetUDPConnection *cur = *itr;
        if (cur->GetAddr() == addr && cur->GetPort() == port) {
            switch (cur->mState) {
                case 0: {
                    cur->mState = 2;
                    func_UnlockShared(&this->mLock);
                    return cur;
                }
                case 1:
                case 2:
                case 3: {
                    cur->mState = 5;
                    break;
                }
                case 5: {
                    break;
                }
                default:
                    GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 2395, "c:\\work\\rts\\main\\code\\src\\core\\NetUDPConn.cpp");
            }
        }
    }
    auto conn = new Moho::CNetUDPConnection{this, addr, port, 1};
    func_UnlockShared(&this->mLock);
    return conn;
}

// 0x0048B410
bool Moho::CNetUDPConnector::FindNextAddr(__out u_long &addr, __out u_short &port) {
    func_LockShared(&this->mLock);
    for (auto itr = this->mConnections.begin(); itr != this->mConnections.end(); ++itr) {
        Moho::CNetUDPConnection *conn = *itr;
        if (! conn->mState && ! conn->mScheduleDestroy) { 
            addr = conn->GetAddr();
            port = conn->GetPort();
            func_UnlockShared(&this->mLock);
            return true;            
        }
    }
    func_UnlockShared(&this->mLock);
    return false;
}

// 0x0048B4F0
Moho::INetConnection *Moho::CNetUDPConnector::Accept(u_long, u_short) {

}

// 0x0048B500
void Moho::CNetUDPConnector::Reject(u_long, u_short) {

}

// 0x0048B5C0
void Moho::CNetUDPConnector::Pull() {

}

// 0x0048B7F0
void Moho::CNetUDPConnector::Push() {

}

// 0x0048B9A0
void Moho::CNetUDPConnector::SelectEvent(HANDLE) {

}

// 0x0048B8E0
void Moho::CNetUDPConnector::Debug() {

}

// 0x0048B9E0
void *Moho::CNetUDPConnector::Func3() {

}

// 0x0048BA80
void Moho::CNetUDPConnector::Func1(Moho::CMessage *msg) {

}

// 0x0048BAE0
void Moho::CNetUDPConnector::ReceivePacket(u_long addr, u_short port, void *dat, size_t size) {

}


// 0x004896F0
Moho::CNetUDPConnector::CNetUDPConnector(SOCKET sock, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {

}

// 0x00489ED0
void Moho::CNetUDPConnector::AddPacket() {

}

// 0x00489F30
__int64 Moho::CNetUDPConnector::GetTime() {

}

// 0x00489F90
void Moho::CNetUDPConnector::Entry(boost::shared_ptr<void>) {

}

// 0x0048A280
__int64 Moho::CNetUDPConnector::ReceiveData() {

}

// 0x0048AA40
void Moho::CNetUDPConnector::ProcessConnect(Moho::SPacket *pack, u_long addr, u_short port) {

}



// 0x00485BE0
u_long Moho::CNetUDPConnection::GetAddr() {

}

// 0x00485BF0
u_short Moho::CNetUDPConnection::GetPort() {

}

// 0x00489550
float Moho::CNetUDPConnection::GetPing() {

}

// 0x00489590
float Moho::CNetUDPConnection::GetTime() {

}

// 0x00489130
void Moho::CNetUDPConnection::Write(struct_DataSpan *data) {

}

// 0x004893F0
void Moho::CNetUDPConnection::Close() {

}

// 0x004894C0
std::string Moho::CNetUDPConnection::ToString() {

}

// 0x00489660
void Moho::CNetUDPConnection::ScheduleDestroy() {

}


// 0x00485D30
Moho::CNetUDPConnection::CNetUDPConnection() {

}

// 0x00486150
Moho::CNetUDPConnection::~CNetUDPConnection() {

}

// 0x00486380
bool Moho::CNetUDPConnection::ProcessConnect(Moho::SPacket *pack) {

}

// 0x004865E0
void Moho::CNetUDPConnection::ProcessAnswer(Moho::SPacket *pack) {

}

// 0x00486910
void Moho::CNetUDPConnection::CreateFilterStream() {

}

// 0x00486B10
bool Moho::CNetUDPConnection::ProcessAck(Moho::SPacket *pack) {

}

// 0x00486DB0
void Moho::CNetUDPConnection::ProcessData(Moho::SPacket *pack) {

}

// 0x00487310
void Moho::CNetUDPConnection::ProcessKeepAlive(Moho::SPacket *pack) {

}

// 0x00487340
void Moho::CNetUDPConnection::ProcessGoodbye(Moho::SPacket *pack) {

}

// 0x00487370
void Moho::CNetUDPConnection::ProcessNATTraversal(Moho::SPacket *pack) {

}

// 0x00488170
__int64 Moho::CNetUDPConnection::CalcResendDelay(Moho::SPacket *pack) {

}

// 0x00488260
int Moho::CNetUDPConnection::GetSentTime(__int64 time) {

}

// 0x00488300
int Moho::CNetUDPConnection::SendData() {

}

// 0x00488730
bool Moho::CNetUDPConnection::HasPacketWaiting(__int64 time) {

}

// 0x00488810
Moho::SPacket *Moho::CNetUDPConnection::NextPacket_0() {

}

// 0x004888C0
Moho::SPacket *Moho::CNetUDPConnection::NextPacket_1() {

}

// 0x00488980
Moho::SPacket *Moho::CNetUDPConnection::ReadPacket() {

}

// 0x00488AA0
Moho::SPacket *Moho::CNetUDPConnection::NextPacket_7() {

}

// 0x00488B20
Moho::SPacket *Moho::CNetUDPConnection::NextPacket(char state, int size, bool inherit) {

}

// 0x00488D80
void Moho::CNetUDPConnection::SendPacket(Moho::SPacket *pack) {

}


// 0x0048BBE0
Moho::INetConnector *Moho::NET_MakeUDPConnector(u_short port, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        return nullptr;
    }
    u_long argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        }
        ::closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(0);
    if (::bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        }
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetUDPConnector{sock, prov};
}
