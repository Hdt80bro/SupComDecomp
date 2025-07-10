#include <string>
#include <winsock.h>
#include "gpgcore/streams/Stream.h"
#include "boost/weak_ptr.hpp"
#include "Message.h"

namespace Moho {

    int net_DebugLevel;

    enum ENetProtocol
    {
        NETPROTO_None = 0x0,
        NETPROTO_TCP = 0x1,
        NETPROTO_UDP = 0x2,
    };

    class INetConnector
    {

    };

    class CNetNullConnector : public Moho::INetConnector
    {};

    class INetDatagramHandler
    {

    };

    class INetConnection : public Moho::CMessageDispatcher
    {
    public:
        DWORD v259;
        DWORD v260;
        DWORD v261;
    };

    class INetDatagramSocket
    {

    };

    class CNetDatagramSocketImpl : public Moho::INetDatagramSocket
    {
    public:
        Moho::INetDatagramHandler *datagramHandler;
        SOCKET socket;
        HANDLE event;

        CNetDatagramSocketImpl(Moho::INetDatagramHandler *handler, SOCKET sock); // inline 0x0047F44E
    };

    class INetTCPSocket : public gpg::Stream
    {
    public:
        SOCKET socket;

        virtual u_short GetPort() = 0;
        virtual u_long GetPeerAddr() = 0;
        virtual u_short GetPeerPort() = 0;

        INetTCPSocket() = default; // 0x004827E0
    };

    class INetTCPServer
    {
    public:
        virtual ~INetTCPServer() = default; // 0x00482750
        virtual unsigned short GetLocalPort() = 0;
        virtual Moho::INetTCPSocket *Accept() = 0;
        virtual void CloseSocket() = 0;

        INetTCPServer() = default; // 0x00482740
    };

    class INetNATTraversalProvider
    {

    };

    class INetNATTraversalHandler
    {

    };

    Moho::INetConnector *NET_MakeConnector(unsigned short, Moho::ENetProtocol, boost::weak_ptr<Moho::INetNATTraversalProvider> *natTravProv); // 0x0047EBF0
    std::string NET_GetProtocolName(Moho::ENetProtocol netproto); // 0x0047EC90
    Moho::ENetProtocol NET_ProtocolFromString(const char *str); // 0x0047ED50
    Moho::INetDatagramSocket *NET_OpenDatagramSocket(unsigned short hostshort, Moho::INetDatagramHandler *datagramHandler); // 0x0047F360
    bool NET_Init(); // 0x0047F5A0
    const char *NET_GetWinsockErrorString(); // 0x0047F5F0
    std::string NET_GetHostName(unsigned long hostlong); // 0x0047FEE0
    bool NET_GetAddrInfo(const char *str, unsigned short unk, bool isTCP, unsigned int &addr, unsigned short &port); // 0x0047FF10
    std::string NET_GetDottedOctetFromUInt32(unsigned int octets); // 0x004801C0
    int Moho::NET_GetUInt32FromDottedOcted/*sic*/(std::string octet); // 0x00480200

    Moho::INetConnector *NET_MakeTCPConnector(unsigned short); // 0x004849A0
    Moho::INetConnector *NET_MakeUDPConnector(unsigned short, boost::weak_ptr<Moho::INetNATTraversalProvider> *); // 0x0048BBE0
};
