#include "core/NetConnector.h"

namespace Moho {

class CNetUDPConnector :
    public Moho::INetConnector, // 0x00E060D4
    public Moho::INetNATTraversalHandler // 0x00E0610C
{
public:
    struct_shared_lock lock;
    SOCKET socket;
    HANDLE eventObject;
    boost::weak_ptr<Moho::INetNATTraversalHandler *> natTravProv;
    Moho::CNetUDPConnection *connectionsEnd;
    Moho::CNetUDPConnection *connectionsStart;
    DWORD v11;
    DWORD v12;
    DWORD packetPoolSize;
    _FILETIME v14;
    gpg::time::Timer timer;
    DWORD v18;
    DWORD v19;
    DWORD v20;
    DWORD v21;
    DWORD v22;
    DWORD v23;
    DWORD v24;
    DWORD v25;
    DWORD v26;
    DWORD v27;
    DWORD v28;
    DWORD v29;
    DWORD v30;
    DWORD v31;
    DWORD gap2[24564];
    gpg::PipeStream stream;
};

// 0x00E06118
class CNetUDPConnection : public Moho::INetConnection
{
public:
    Moho::CNetUDPConnector *connector;
    DWORD Remote;
    unsigned __int16 RemoteAddr;
    WORD gap1;
    enum_NetCompressionMethod compressionMethod;
    DWORD v266;
    DWORD state;
    gpg::time::Timer LastSend;
    int gap438;
    DWORD abc1;
    _QWORD LastRecv;
    DWORD LastKeepAlive;
    DWORD abc3;
    DWORD KeepAliveFreq;
    DWORD v277;
    DWORD v278;
    DWORD v279;
    DWORD v280;
    DWORD v281;
    DWORD v282;
    DWORD v283;
    DWORD v284;
    DWORD v285;
    DWORD v286;
    DWORD v287;
    DWORD v288;
    DWORD v289;
    DWORD v290;
    DWORD v291;
    DWORD v292;
    DWORD v293;
    DWORD v294;
    DWORD v295;
    unsigned __int16 NextSerialNumber;
    unsigned __int16 ResponseTo;
    gpg::time::Timer SendBy;
    unsigned __int16 NextSequenceNumber;
    unsigned __int16 RemoteExpectedSequenceNumber;
    unsigned __int16 ExpectedSequenceNumber;
    unsigned __int16 v1;
    gpg::PipeStream OutputData;
    gpg::ZLibOutputFilterStream *OutputFilterStream;
    _BYTE gap500[4];
    DWORD FlushedOutputData;
    _BYTE OutputShutdown;
    _BYTE SentShutdown;
    _WORD gap50E;
    struct_UDPPacket *UnackedPayloadsEnd;
    struct_UDPPacket *UnackedPayloadsStart;
    struct_packet_time timings[128];
    DWORD v838;
    DWORD v839;
    DWORD v840;
    DWORD v841;
    DWORD v842;
    DWORD v843;
    DWORD v844;
    DWORD v845;
    DWORD v846;
    DWORD v847;
    DWORD v848;
    DWORD v849;
    DWORD v850;
    DWORD v851;
    DWORD v852;
    DWORD v853;
    DWORD v854;
    DWORD v855;
    DWORD v856;
    DWORD v857;
    DWORD v858;
    DWORD v859;
    DWORD v860;
    DWORD v861;
    DWORD v862;
    DWORD v863;
    DWORD v864;
    DWORD v865;
    float PingTime;
    struct_UDPPacket *EarlyPacketsEnd;
    struct_UDPPacket *EarlyPacketsStart;
    DWORD gapD94;
    gpg::PipeStream InputBuffer;
    _BYTE gapDE0[4];
    _BYTE ReceivedEndOfInput;
    _BYTE DispatchedEndOfInput;
    _BYTE gapDE6[90];
    DWORD v866;
    _BYTE Closed;
    _QWORD TotalBytesQueued;
    _QWORD TotalBytesSent;
    _QWORD TotalBytesReceived;
    _QWORD TotalBytesDispatched;
    gpg::MD5Context TotalBytesQueuedMD5;
    gpg::MD5Context TotalBytesSentMD5;
    gpg::MD5Context TotalBytesReceivedMD5;
    gpg::MD5Context TotalBytesDispatchedMD5;
};

}
