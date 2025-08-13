#include "NetConnector.h"


void *Moho::INetConnector::Debug() {}


void Moho::CNetNullConnector::Destroy() {
    delete this;
}
Moho::ENetProtocol Moho::CNetNullConnector::GetProtocol() {
    return Moho::NETPROTO_None;
}
int Moho::CNetNullConnector::GetLocalPort() {
    return 0;
}
Moho::INetConnection *Moho::CNetNullConnector::Connect(u_long, u_short) {
    return nullptr;
}
bool Moho::CNetNullConnector::Func2(u_long &, u_short &) {
    return false;
}
Moho::INetConnection *Moho::CNetNullConnector::Accept(u_long, u_short) {
    return nullptr;
}
void Moho::CNetNullConnector::Reject(u_long, u_short) {}
void Moho::CNetNullConnector::Pull() {}
void Moho::CNetNullConnector::Push() {}
void Moho::CNetNullConnector::SelectEvent(HANDLE) {}
void *Moho::CNetNullConnector::Func3() {
    return nullptr; // unknown type;
}




Moho::CNetDatagramSocketImpl::~CNetDatagramSocketImpl() {
    closesocket(this->mSocket);
    if (this->mEvent) {
        WSACloseEvent(this->mEvent);
    }
}
void Moho::CNetDatagramSocketImpl::SendDefault(Moho::CMessage *dat, u_short port) {
    this->Send(dat, -1, port);
}
void Moho::CNetDatagramSocketImpl::Send(Moho::CMessage *dat, u_long addr, u_short port) {
    sockaddr_in to;
    ZeroMemory(&to, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_port = htons(port);
    to.sin_addr.S_un.S_addr = htonl(addr);
    int res = sendto(this->mSocket, dat->mBuf.mStart, dat->mBuf.Size(), 0, (SOCKADDR *) &to, sizeof(to));
    if (res == SOCKET_ERROR) {
        gpg::Logf("CNetDatagramSocketImpl::Send: send() failed: %s", Moho::NET_GetWinsockErrorString());
    } else if (res < dat->mBuf.Size()) {
        gpg::Logf("CNetDatagramSocketImpl::Send: msg truncated, only %d of %d bytes sent.", res, dat->mBuf.Size());
    }
}
void Moho::CNetDatagramSocketImpl::Pull() {
    if (this->mEvent) {
        WSAResetEvent(this->mEvent);
    }
    char buf[2048];
    while (true) {
        sockaddr_in from;
        ZeroMemory(&from, sizeof(from));
        int fromlen = sizeof(from);
        int res = recvfrom(this->mSocket, buf, sizeof(buf), 0, (SOCKADDR *) &from, &fromlen);
        if (res == SOCKET_ERROR) {
            break;
        }

        Moho::CMessage dat{sizeof(buf), 0};
        memcpy(dat.mBuf.mStart, buf, res);
        u_short port = ntohs(from.sin_port);
        u_long addr = ntohl(from.sin_addr.S_un.S_addr);
        this->mDatagramHandler->Pull(&dat, this, addr, port);
    }
    if (WSAGetLastError() != WSAEWOULDBLOCK) {
        gpg::Logf("CNetBroadcastSocketImpl::Pull: recv() failed: %s", Moho::NET_GetWinsockErrorString());
    }
}
HANDLE Moho::CNetDatagramSocketImpl::CreateEvent() {
    if (! this->mEvent) {
        this->mEvent = WSACreateEvent();
        WSAEventSelect(this->mSocket, this->mEvent, FD_READ);
    }
    return this->mEvent;
}

Moho::CNetDatagramSocketImpl::CNetDatagramSocketImpl(Moho::INetDatagramHandler *handler, SOCKET sock) :
    Moho::INetDatagramSocket{},
    mDatagramHandler{handler},
    mSocket{sock},
    mEvent{0}
{}


Moho::INetConnector *Moho::NET_MakeConnector(u_short host, Moho::ENetProtocol prot, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {
    switch (prot) {
        case NETPROTO_TCP: return Moho::NET_MakeTCPConnector(host);
        case NETPROTO_UDP: return Moho::NET_MakeUDPConnector(host, prov);
        default: return new CNetNullConnector();
    }
}

bool Moho::NET_Init() {
    static bool sWinsockInitialized = false;
    if (! sWinsockInitialized) {
        WSAData WSAData;
        if (WSAStartup(MAKEWORD(1, 1), &WSAData)) {
            gpg::Logf("Net_Init(): WSAStartup failed: %s", Moho::NET_GetWinsockErrorString());
        } else {
            sWinsockInitialized = 1;
        }
    }
    return sWinsockInitialized;
}

std::string Moho::NET_GetProtocolName(Moho::ENetProtocol netproto) {
    switch (netproto) {
        case NETPROTO_None: return std::string{"None"};
        case NETPROTO_TCP: return std::string{"TCP"};
        case NETPROTO_UDP: return std::string{"UDP"};
        default: throw std::domain_error{gpg::STR_Printf("invalid protocol (%d)", netproto)};
    }
}

Moho::ENetProtocol Moho::NET_ProtocolFromString(const char *str) {
    if (stricmp(str, "None") == 0) {
        return Moho::NETPROTO_None;
    } else if (stricmp(str, "TCP") == 0) {
        return Moho::NETPROTO_TCP;
    } else if (stricmp(str, "UDP") == 0) {
        return Moho::NETPROTO_UDP;
    } else {
        throw std::domain_error{gpg::STR_Printf("invalid protocol (\"%s\")", str)};
    }
}

Moho::INetDatagramSocket *Moho::NET_OpenDatagramSocket(unsigned short hostshort, Moho::INetDatagramHandler *datagramHandler) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_OpenDatagramSocket: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        return 0;
    }
    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) optval, sizeof(int)) == SOCKET_ERROR) {
        gpg::Logf("NET_OpenDatagramSocket: setsockopt(SO_BROADCAST) failed : %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    unsigned long argp = 1;
    if (ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("NET_OpenDatagramSocket: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(hostshort);
    name.sin_addr.S_un.S_addr = htonl(0);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_OpenDatagramSocket: bind() failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetDatagramSocketImpl{datagramHandler, 0};
}

int Moho::NET_GetUInt32FromDottedOcted(std::string octet) {
    int val = 0;
    const char *cOctet = octet.c_str();
    std::string token;
    while (gpg::STR_GetToken(&cOctet, ".", &token)) {
        val = atoi(token.c_str()) | (val << 8);
    }
    return val;
}

std::string Moho::NET_GetDottedOctetFromUInt32(unsigned int octets) {
    return gpg::STR_Printf("%d.%d.%d.%d", HIWORD(HIBYTE(octets)), HIWORD(LOBYTE(octets)), HIBYTE(octets), LOBYTE(octets));
}

bool Moho::NET_GetAddrInfo(const char *str, unsigned short unk, bool isTCP, unsigned int &addr, unsigned short &port) {
    static WCHAR message_format_buffer[0x200];

    Moho::NET_Init();
    std::string nodeName;
    std::string serviceName;
    const char *last = strrchr(str, ':');
    if (last) {
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
    int res = getaddrinfo(nodeName.c_str(), serviceName.c_str(), &hints, &pAddrinfo);
    if (res) {
        FormatMessageW(
            FORMAT_MESSAGE_MAX_WIDTH_MASK|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
            0, res,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            message_format_buffer, sizeof(message_format_buffer), nullptr
        );
        gpg::Logf("getaddrinfo(%s,%s) failed: %s [ret=%d]",
            nodeName.c_str(),
            serviceName.c_str(),
            (const char *) message_format_buffer, res
        );
        return false;
    } else {
        addr = ntohl(pAddrinfo->ai_addr->sin_addr.S_un.S_addr);
        port = htons(pAddrinfo->ai_addr->sin_port);
        freeaddrinfo(pAddrinfo);
        return true;
    }
}


const char *Moho::NET_GetWinsockErrorString() {
    switch (WSAGetLastError()) {
        case NOERROR:     return "NOERROR";
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
        default:    return "UNKNOWN";
    }
}
