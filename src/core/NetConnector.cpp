#include "NetConnector.h"


struct_Datagram::struct_Datagram(int size, char type) {
    size += 3;
    char fill = 0;
    this->buf.Resize(size, &fill);       
    this->SetSize(size);
    this->SetType(type);
}

int struct_Datagram::GetMessageSize() {
    int size = this->GetSize();
    if (size >= 3) {
        size -= 3;
    }
    return size;
}

unsigned int struct_Datagram::Append(char *ptr, int size) {
    if (this->buf.Size() + size > 0x10000) {
        throw std::runtime_error{std::string{"Message too large"}};
    }
    this->buf.InsertAt(this->buf.end, ptr, &ptr[size]);
    this->SetSize(this->buf.Size());
}

bool struct_Datagram::ReadMessage(gpg::Stream *stream) {
    char fill = 0;
    this->buf.Resize(3, &fill);
    if (stream->Read(this->buf.start, 3) != 3) {
        return false;
    }
    int size = this->GetSize();
    if (size < 3) {
        return false;
    }
    if (size == 3) {
        return true;
    }
    this->buf.Resize(size - 3, &fill);
    return stream->Read(&this->buf[3], size - 3) == size - 3;
}

bool struct_Datagram::Read(gpg::Stream *stream) {
    if (! this->HasReadLength()) {
        if (this->buf.Size() == 0) {
            char fill = 0;
            this->buf.Resize(3, &fill);
        }
        this->pos += stream->ReadNonBlocking(&this->buf[this->pos], 3 - this->pos);
        if (! this->HasReadLength()) {
            return false;
        }
    }
    int newSize = this->GetSize();
    if (newSize < 3) {
        return false;
    }
    if (newSize == this->pos) {
        return true;
    }
    char fill = 0;
    this->buf.Resize(newSize, &fill);
    this->pos += stream->ReadNonBlocking(&this->buf[this->pos], newSize - this->pos);
    return this->pos == newSize;
}


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
    closesocket(this->socket);
    if (this->event ) {
        WSACloseEvent(this->event);
    }
}
void Moho::CNetDatagramSocketImpl::SendDefault(struct_Datagram *dat, u_short port) {
    this->Send(dat, -1, port);
}
void Moho::CNetDatagramSocketImpl::Send(struct_Datagram *dat, u_long addr, u_short port) {
    sockaddr_in to;
    ZeroMemory(&to, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_port = htons(port);
    to.sin_addr.S_un.S_addr = htonl(addr);
    int res = sendto(this->socket, dat->buf.start, dat->buf.Size(), 0, (SOCKADDR *) &to, sizeof(to));
    if (res == SOCKET_ERROR) {
        gpg::Logf("CNetDatagramSocketImpl::Send: send() failed: %s", Moho::NET_GetWinsockErrorString());
    } else if (res < dat->buf.Size()) {
        gpg::Logf("CNetDatagramSocketImpl::Send: msg truncated, only %d of %d bytes sent.", res, dat->buf.Size());
    }
}
void Moho::CNetDatagramSocketImpl::Pull() {
    if (this->event) {
        WSAResetEvent(this->event);
    }
    char buf[2048];
    while (true) {
        sockaddr_in from;
        ZeroMemory(&from, sizeof(from));
        int fromlen = sizeof(from);
        int res = recvfrom(this->socket, buf, sizeof(buf), 0, (SOCKADDR *) &from, &fromlen);
        if (res == SOCKET_ERROR) {
            break;
        }

        struct_Datagram dat{sizeof(buf), 0};
        memcpy(dat.start, buf, res);
        u_short port = ntohs(from.sin_port);
        u_long addr = ntohl(from.sin_addr.S_un.S_addr);
        this->datagramHandler->Pull(&dat, this, addr, port);
    }
    if (WSAGetLastError() != WSAEWOULDBLOCK) {
        gpg::Logf("CNetBroadcastSocketImpl::Pull: recv() failed: %s", Moho::NET_GetWinsockErrorString());
    }
}
HANDLE Moho::CNetDatagramSocketImpl::CreateEvent() {
    if (! this->event) {
        this->event = WSACreateEvent();
        WSAEventSelect(this->socket, this->event, FD_READ);
    }
    return this->event;
}

Moho::CNetDatagramSocketImpl::CNetDatagramSocketImpl(Moho::INetDatagramHandler *handler, SOCKET sock) :
    Moho::INetDatagramSocket{},
    datagramHandler{handler},
    socket{sock},
    event{nullptr}
{}


Moho::INetConnector *Moho::NET_MakeConnector(unsigned short host, Moho::ENetProtocol prot, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {
    switch (prot) {
        case NETPROTO_TCP: return Moho::NET_MakeTCPConnector(host);
        case NETPROTO_UDP: return Moho::NET_MakeUDPConnector(host, prov);
        default: return new CNetNullConnector();
    }
}

bool Moho::NET_Init() {
    static bool winsock_initialized = false;
    if (! winsock_initialized) {
        WSAData WSAData;
        if (WSAStartup(MAKEWORD(1, 1), &WSAData)) {
            gpg::Logf("Net_Init(): WSAStartup failed: %s", Moho::NET_GetWinsockErrorString());
        } else {
            winsock_initialized = 1;
        }
    }
    return winsock_initialized;
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
        case 0:     return "NOERROR";
        case 10004: return "WSAEINTR";
        case 10009: return "WSAEBADF";
        case 10013: return "WSAEACCES";
        case 10014: return "WSAEFAULT";
        case 10022: return "WSAEINVAL";
        case 10024: return "WSAEMFILE";
        case 10035: return "WSAEWOULDBLOCK";
        case 10036: return "WSAEINPROGRESS";
        case 10037: return "WSAEALREADY";
        case 10038: return "WSAENOTSOCK";
        case 10039: return "WSAEDESTADDRREQ";
        case 10040: return "WSAEMSGSIZE";
        case 10041: return "WSAEPROTOTYPE";
        case 10042: return "WSAENOPROTOOPT";
        case 10043: return "WSAEPROTONOSUPPORT";
        case 10044: return "WSAESOCKTNOSUPPORT";
        case 10045: return "WSAEOPNOTSUPP";
        case 10046: return "WSAEPFNOSUPPORT";
        case 10047: return "WSAEAFNOSUPPORT";
        case 10048: return "WSAEADDRINUSE";
        case 10049: return "WSAEADDRNOTAVAIL";
        case 10050: return "WSAENETDOWN";
        case 10051: return "WSAENETUNREACH";
        case 10052: return "WSAENETRESET";
        case 10053: return "WSAECONNABORTED";
        case 10054: return "WSAECONNRESET";
        case 10055: return "WSAENOBUFS";
        case 10056: return "WSAEISCONN";
        case 10057: return "WSAENOTCONN";
        case 10058: return "WSAESHUTDOWN";
        case 10059: return "WSAETOOMANYREFS";
        case 10060: return "WSAETIMEDOUT";
        case 10061: return "WSAECONNREFUSED";
        case 10062: return "WSAELOOP";
        case 10063: return "WSAENAMETOOLONG";
        case 10064: return "WSAEHOSTDOWN";
        case 10065: return "WSAEHOSTUNREACH";
        case 10066: return "WSAENOTEMPTY";
        case 10067: return "WSAEPROCLIM";
        case 10068: return "WSAEUSERS";
        case 10069: return "WSAEDQUOT";
        case 10070: return "WSAESTALE";
        case 10071: return "WSAEREMOTE";
        case 10091: return "WSASYSNOTREADY";
        case 10092: return "WSAVERNOTSUPPORTED";
        case 10093: return "WSANOTINITIALISED";
        case 10101: return "WSAEDISCON";
        case 11001: return "WSAHOST_NOT_FOUND";
        case 11002: return "WSATRY_AGAIN";
        case 11003: return "WSANO_RECOVERY";
        default:    return "UNKNOWN";
    }
}
