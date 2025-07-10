#include "NetUDPConn.h"

Moho::INetConnector *Moho::NET_MakeUDPConnector(unsigned short port, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        return nullptr;
    }
    unsigned long argp = 1;
    if (ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        }
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(0);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        }
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetUDPConnector(sock, port);
}