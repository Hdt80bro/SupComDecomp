#include "NetConnector.h"
#include "core/NetTCPConn.h"
#include "core/NetUDPConn.h"


// 0x0047EAD0
void Moho::INetConnector::Debug() {}


// 0x0047EB20
void Moho::CNetNullConnector::Destroy() {
    delete(this);
}

// 0x0047EB30
Moho::ENetProtocol Moho::CNetNullConnector::GetProtocol() {
    return Moho::NETPROTO_None;
}

// 0x0047EB40
int Moho::CNetNullConnector::GetLocalPort() {
    return 0;
}

// 0x0047EB50
Moho::INetConnection *Moho::CNetNullConnector::Connect(u_long, u_short) {
    return nullptr;
}

// 0x0047EB60
bool Moho::CNetNullConnector::FindNextAddr(u_long &, u_short &) {
    return false;
}

// 0x0047EB70
Moho::INetConnection *Moho::CNetNullConnector::Accept(u_long, u_short) {
    return nullptr;
}

// 0x0047EB80
void Moho::CNetNullConnector::Reject(u_long, u_short) {}

// 0x0047EB90
void Moho::CNetNullConnector::Pull() {}

// 0x0047EBA0
void Moho::CNetNullConnector::Push() {}

// 0x0047EBA0
void Moho::CNetNullConnector::SelectEvent(HANDLE) {}

// 0x0047EBC0
void *Moho::CNetNullConnector::Func3() {
    return nullptr; // unknown type;
}


// 0x0047F050
Moho::CNetDatagramSocketImpl::~CNetDatagramSocketImpl() {
    ::closesocket(this->mSocket);
    if (this->mEvent) {
        ::WSACloseEvent(this->mEvent);
    }
}

// 0x0047F0D0
void Moho::CNetDatagramSocketImpl::SendDefault(Moho::CMessage *dat, u_short port) {
    this->Send(dat, -1, port);
}

// 0x0047F0F0
void Moho::CNetDatagramSocketImpl::Send(Moho::CMessage *dat, u_long addr, u_short port) {
    sockaddr_in to;
    ZeroMemory(&to, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_port = ::htons(port);
    to.sin_addr.S_un.S_addr = ::htonl(addr);
    int res = ::sendto(this->mSocket, dat->mBuf.mStart, dat->mBuf.Size(), 0, (SOCKADDR *) &to, sizeof(to));
    if (res == SOCKET_ERROR) {
        gpg::Logf("CNetDatagramSocketImpl::Send: send() failed: %s", Moho::NET_GetWinsockErrorString());
    } else if (res < dat->mBuf.Size()) {
        gpg::Logf("CNetDatagramSocketImpl::Send: msg truncated, only %d of %d bytes sent.", res, dat->mBuf.Size());
    }
}

// 0x0047F190
void Moho::CNetDatagramSocketImpl::Pull() {
    if (this->mEvent) {
        ::WSAResetEvent(this->mEvent);
    }
    char buf[2048];
    while (true) {
        sockaddr_in from;
        ZeroMemory(&from, sizeof(from));
        int fromlen = sizeof(from);
        int res = ::recvfrom(this->mSocket, buf, sizeof(buf), 0, (SOCKADDR *) &from, &fromlen);
        if (res == SOCKET_ERROR) {
            break;
        }

        Moho::CMessage dat{sizeof(buf), 0};
        memcpy(dat.mBuf.mStart, buf, res);
        u_short port = ::ntohs(from.sin_port);
        u_long addr = ::ntohl(from.sin_addr.S_un.S_addr);
        this->mDatagramHandler->Pull(&dat, this, addr, port);
    }
    if (WSAGetLastError() != WSAEWOULDBLOCK) {
        gpg::Logf("CNetBroadcastSocketImpl::Pull: recv() failed: %s", Moho::NET_GetWinsockErrorString());
    }
}

// 0x0047F330
HANDLE Moho::CNetDatagramSocketImpl::CreateEvent() {
    if (! this->mEvent) {
        this->mEvent = ::WSACreateEvent();
        ::WSAEventSelect(this->mSocket, this->mEvent, FD_READ);
    }
    return this->mEvent;
}

// inline 0x0047F44E
Moho::CNetDatagramSocketImpl::CNetDatagramSocketImpl(Moho::INetDatagramHandler *handler, SOCKET sock) :
    Moho::INetDatagramSocket{},
    mDatagramHandler{handler},
    mSocket{sock},
    mEvent{0}
{}


// 0x0047EBF0
Moho::INetConnector *Moho::NET_MakeConnector(u_short host, Moho::ENetProtocol prot, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {
    switch (prot) {
        case NETPROTO_TCP: return Moho::NET_MakeTCPConnector(host);
        case NETPROTO_UDP: return Moho::NET_MakeUDPConnector(host, prov);
        default: return new CNetNullConnector{};
    }
}

// 0x0047EC90
std::string Moho::NET_GetProtocolName(Moho::ENetProtocol netproto) {
    switch (netproto) {
        case NETPROTO_None: return std::string{"None"};
        case NETPROTO_TCP: return std::string{"TCP"};
        case NETPROTO_UDP: return std::string{"UDP"};
        default: throw std::domain_error{gpg::STR_Printf("invalid protocol (%d)", netproto)};
    }
}

// 0x0047ED50
Moho::ENetProtocol Moho::NET_ProtocolFromString(const char *str) {
    if (stricmp(str, "None") == 0) {
        return Moho::ENetProtocol::NETPROTO_None;
    } else if (stricmp(str, "TCP") == 0) {
        return Moho::ENetProtocol::NETPROTO_TCP;
    } else if (stricmp(str, "UDP") == 0) {
        return Moho::ENetProtocol::NETPROTO_UDP;
    } else {
        throw std::domain_error{gpg::STR_Printf("invalid protocol (\"%s\")", str)};
    }
}

// 0x0047F360
Moho::INetDatagramSocket *Moho::NET_OpenDatagramSocket(unsigned short hostshort, Moho::INetDatagramHandler *datagramHandler) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_OpenDatagramSocket: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        return 0;
    }
    int optval = 1;
    if (::setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) optval, sizeof(int)) == SOCKET_ERROR) {
        gpg::Logf("NET_OpenDatagramSocket: setsockopt(SO_BROADCAST) failed : %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    unsigned long argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("NET_OpenDatagramSocket: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(hostshort);
    name.sin_addr.S_un.S_addr = ::htonl(0);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_OpenDatagramSocket: bind() failed: %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetDatagramSocketImpl{datagramHandler, 0};
}

// 0x0047F5A0
bool Moho::NET_Init() {
    static bool sWinsockInitialized = false;
    if (! sWinsockInitialized) {
        WSAData WSAData;
        if (::WSAStartup(MAKEWORD(1, 1), &WSAData)) {
            gpg::Logf("Net_Init(): WSAStartup failed: %s", Moho::NET_GetWinsockErrorString());
        } else {
            sWinsockInitialized = true;
        }
    }
    return sWinsockInitialized;
}

// 0x0047FEE0
std::string Moho::NET_GetHostName(u_long addr) {
    return func_NET_GetHostName(func_GetHostManager(), addr);
}

// 0x0047F990
struct_HostManager *func_GetHostManager() {
    static struct_HostManager sHostManager{}; // 0x01104228
    return &sHostManager;
}

// 0x0047FBE0
std::string func_NET_GetHostName(struct_HostManager *mgr, u_long addr) {
    
    boost::mutex::do_lock(&mgr->mLock);
    auto node = mgr->mHosts.find(addr);
    if (node == mgr->mHosts._Myhead) {
        boost::mutex::unlock(&mgr->mLock);
        Moho::NET_Init();
        sockaddr_in sockaddr;
        ZeroMemory(sockaddr, sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = ::htons(0);
        sockaddr.sin_addr.S_un.S_addr = ::htonl(addr);
        std::string str1{};
        char pServiceBuffer[32];
        CHAR pNodeBuffer[1036];
        if (::getnameinfo((SOCKADDR *) &sockaddr, 16, pNodeBuffer, 0x401u, pServiceBuffer, 0x20u, 8) ) {
            gpg::Logf("NET_GetHostName: getnameinfo() failed: %s", Moho::NET_GetWinsockErrorString());
            str1.assign(Moho::NET_GetDottedOctetFromUInt32(addr));
        } else {
            str1.assign(pNodeBuffer);
        }
        auto inited = new struct_Host{&str1};
        boost::mutex::do_lock(&mgr->mLock);
        v12 = sub_4803F0(&mgr->mHosts, (int)v27, (unsigned int *)&v21);
        v13 = *(std::map_uint_Host::_Node **)v12;
        v18 = *(std::map_uint_Host::_Node **)v12;
        if (*(_BYTE *)(v12 + 4)) {
            mNext = (struct_Host *)inited->mNext;
            inited->mVal = (int)v13;
            inited->mPrev->mNext = mNext;
            inited->mNext->mPrev = inited->mPrev;
            inited->mNext = inited;
            inited->mPrev = &mgr->mHostList;
            v15 = mgr->mHostList.mNext;
            inited->mNext = v15;
            v15->mPrev = inited;
            mPrev = inited->mPrev;
            inited->mPrev->mNext = inited;
            if (mgr->mHosts._Mysize > 0x4C) {
                    sub_4804A0(mPrev, (int)&mgr->mHosts, &v21, (int)mgr->mHostList.mPrev.mVal);
            }
        } else if (inited) {
            sub_47FB50((void **)&inited->mPrev);
        }
        out->_Myres = 15;
        out->_Mysize = 0;
        out->_Bx._Buf[0] = 0;
        out.assign(str1);
    } else {
        val = node->_Myval.val;
        val->mPrev->mNext = val->mNext;
        val->mNext->mPrev = val->mPrev;
        val->mNext = val;
        val->mPrev = &mgr->mHostList;
        v4 = mgr->mHostList.mNext;
        val->mNext = v4;
        v4->mPrev = val;
        val->mPrev->mNext = val;
        out.assign(node->_Myval.val->mName);
    }
    boost::mutex::unlock(&mgr->mLock);
    return out;
}



// 0x0047FF10
bool Moho::NET_GetAddrInfo(const char *str, unsigned short unk, bool isTCP, u_long &addr, u_short &port) {
    static WCHAR sMessageFormatBuffer[0x200];

    Moho::NET_Init();
    std::string nodeName;
    std::string serviceName;
    const char *last = strrchr(str, ':');
    if (last != nullptr) {
        nodeName = std::string{str, last};
        serviceName.append(last + 1, strlen(last + 1));
    } else {
        nodeName = std::string{str, strlen(str)};
        serviceName.assign(gpg::STR_Printf("%d", unk), 0);
    }
    ADDRINFOA hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = isTCP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = isTCP ? IPPROTO_TCP : IPPROTO_UDP;
    PADDRINFOA pAddrinfo;
    int res = ::getaddrinfo(nodeName.c_str(), serviceName.c_str(), &hints, &pAddrinfo);
    if (res != 0) {
        ::FormatMessageW(
            FORMAT_MESSAGE_MAX_WIDTH_MASK|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
            0, res,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            sMessageFormatBuffer, sizeof(sMessageFormatBuffer), nullptr
        );
        gpg::Logf("getaddrinfo(%s,%s) failed: %s [ret=%d]",
            nodeName.c_str(),
            serviceName.c_str(),
            (const char *) sMessageFormatBuffer, res
        );
        return false;
    } else {
        addr = ::ntohl(pAddrinfo->ai_addr->sin_addr.S_un.S_addr);
        port = ::htons(pAddrinfo->ai_addr->sin_port);
        ::freeaddrinfo(pAddrinfo);
        return true;
    }
}

// 0x004801C0
std::string Moho::NET_GetDottedOctetFromUInt32(unsigned int octets) {
    return gpg::STR_Printf("%d.%d.%d.%d", HIWORD(HIBYTE(octets)), HIWORD(LOBYTE(octets)), HIBYTE(octets), LOBYTE(octets));
}

// 0x00480200
int Moho::NET_GetUInt32FromDottedOcted(std::string octet) {
    int val = 0;
    const char *cOctet = octet.c_str();
    std::string token;
    while (gpg::STR_GetToken(&cOctet, ".", &token)) {
        val = atoi(token.c_str()) | (val << 8);
    }
    return val;
}

// 0x0047F5F0
const char *Moho::NET_GetWinsockErrorString() {
    switch (::WSAGetLastError()) {
        case NOERROR: return "NOERROR";
        case WSAEINTR: return "WSAEINTR";
        case WSAEBADF: return "WSAEBADF";
        case WSAEACCES: return "WSAEACCES";
        case WSAEFAULT: return "WSAEFAULT";
        case WSAEINVAL: return "WSAEINVAL";
        case WSAEMFILE: return "WSAEMFILE";
        case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
        case WSAEINPROGRESS: return "WSAEINPROGRESS";
        case WSAEALREADY: return "WSAEALREADY";
        case WSAENOTSOCK: return "WSAENOTSOCK";
        case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
        case WSAEMSGSIZE: return "WSAEMSGSIZE";
        case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
        case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
        case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
        case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
        case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
        case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
        case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
        case WSAEADDRINUSE: return "WSAEADDRINUSE";
        case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
        case WSAENETDOWN: return "WSAENETDOWN";
        case WSAENETUNREACH: return "WSAENETUNREACH";
        case WSAENETRESET: return "WSAENETRESET";
        case WSAECONNABORTED: return "WSAECONNABORTED";
        case WSAECONNRESET: return "WSAECONNRESET";
        case WSAENOBUFS: return "WSAENOBUFS";
        case WSAEISCONN: return "WSAEISCONN";
        case WSAENOTCONN: return "WSAENOTCONN";
        case WSAESHUTDOWN: return "WSAESHUTDOWN";
        case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
        case WSAETIMEDOUT: return "WSAETIMEDOUT";
        case WSAECONNREFUSED: return "WSAECONNREFUSED";
        case WSAELOOP: return "WSAELOOP";
        case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
        case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
        case WSAEHOSTUNREACH: return "WSAEHOSTUNREACH";
        case WSAENOTEMPTY: return "WSAENOTEMPTY";
        case WSAEPROCLIM: return "WSAEPROCLIM";
        case WSAEUSERS: return "WSAEUSERS";
        case WSAEDQUOT: return "WSAEDQUOT";
        case WSAESTALE: return "WSAESTALE";
        case WSAEREMOTE: return "WSAEREMOTE";
        case WSASYSNOTREADY: return "WSASYSNOTREADY";
        case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
        case WSANOTINITIALISED: return "WSANOTINITIALISED";
        case WSAEDISCON: return "WSAEDISCON";
        case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
        case WSATRY_AGAIN: return "WSATRY_AGAIN";
        case WSANO_RECOVERY: return "WSANO_RECOVERY";
        default: return "UNKNOWN";
    }
}
