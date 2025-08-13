#include "core/NetConnector.h"

namespace Moho {

class CNetUDPConnector :
    public Moho::INetConnector, // 0x00E060D4
    public Moho::INetNATTraversalHandler // 0x00E0610C
{
public:
    struct_shared_lock mLock;
    SOCKET mSocket;
    HANDLE mEventObject;
    boost::weak_ptr<Moho::INetNATTraversalHandler *> mNatTravProv;
    Moho::CNetUDPConnection *mConnectionsEnd;
    Moho::CNetUDPConnection *mConnectionsStart;
    DWORD v11;
    DWORD v12;
    DWORD mPacketPoolSize;
    _FILETIME v14;
    gpg::time::Timer mTimer;
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
    gpg::PipeStream mStream;
};

// 0x00E06118
class CNetUDPConnection : public Moho::INetConnection
{
public:
    Moho::CNetUDPConnector *mConnector;
    u_long mAddr;
    u_short mPort;
    WORD gap1;
    enum_NetCompressionMethod compressionMethod;
    DWORD v266;
    DWORD state;
    gpg::time::Timer mLastSend;
    int gap438;
    DWORD abc1;
    _QWORD LastRecv;
    DWORD mLastKeepAlive;
    DWORD abc3;
    DWORD mKeepAliveFreq;
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
    unsigned __int16 mNextSerialNumber;
    unsigned __int16 mResponseTo;
    gpg::time::Timer mSendBy;
    unsigned __int16 mNextSequenceNumber;
    unsigned __int16 mRemoteExpectedSequenceNumber;
    unsigned __int16 mExpectedSequenceNumber;
    unsigned __int16 v1;
    gpg::PipeStream mOutputData;
    gpg::ZLibOutputFilterStream *mOutputFilterStream;
    _BYTE gap500[4];
    DWORD mFlushedOutputData;
    _BYTE mOutputShutdown;
    _BYTE mSentShutdown;
    _WORD gap50E;
    struct_UDPPacket *mUnackedPayloadsEnd;
    struct_UDPPacket *mUnackedPayloadsStart;
    struct_packet_time mTimings[128];
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
    float mPingTime;
    struct_UDPPacket *mEarlyPacketsEnd;
    struct_UDPPacket *mEarlyPacketsStart;
    DWORD gapD94;
    gpg::PipeStream mInputBuffer;
    _BYTE gapDE0[4];
    _BYTE mReceivedEndOfInput;
    _BYTE mDispatchedEndOfInput;
    _BYTE gapDE6[90];
    DWORD v866;
    _BYTE mClosed;
    _QWORD mTotalBytesQueued;
    _QWORD mTotalBytesSent;
    _QWORD mTotalBytesReceived;
    _QWORD mTotalBytesDispatched;
    gpg::MD5Context mTotalBytesQueuedMD5;
    gpg::MD5Context mTotalBytesSentMD5;
    gpg::MD5Context mTotalBytesReceivedMD5;
    gpg::MD5Context mTotalBytesDispatchedMD5;
};

}
