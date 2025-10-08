#include "user/LaunchInfo.h"
#include "core/Message.h"
#include "core/Set.h"
#include "core/NetConnector.h"
#include "gpgcore/streams/PipeStream.h"
#include "boost/thread.hpp"
#include <string>
#include <vector>


namespace Moho {

struct SEjectRequest
{
    const char *mRequester;
    int mAfterBeat;
};

struct SClientBottleneckInfo
{
    enum Type
    {
        Nothing = 0,
        Readiness = 1,
        Data = 2,
        Ack = 3,
    };

    Moho::SClientBottleneckInfo::Type mType;
    int mVal;
    Moho::BVIntSet mSubobj;
    float mFloat;
};

class IClientManager;


// 0x00E16ABC
class IClient
{
public:
    std::string mNickname;
    int mIndex;
    Moho::LaunchInfoBase *mLaunchInfo;

    virtual Moho::BVIntSet *GetValidCommandSources() = 0;
    virtual bool NoEjectionPending() = 0;
    virtual float Func2() = 0;
    virtual float Func3() = 0;
    virtual int &GetLatestAckReceived(int &out) = 0;
    virtual int &GetLatestBeatDispatchedRemote(int &out) = 0;
    virtual int &GetAvailableBeatRemote(int &out) = 0;
    virtual void Process() = 0;
    virtual void ReceiveChat() = 0;
    virtual int &GetQueuedBeat(int &out) = 0;
    virtual void Eject() = 0;
    virtual void Func9() = 0;
    virtual int GetSimRate() = 0;
    virtual ~IClient() = 0;
    virtual void Destroy() = 0;
    virtual void Debug() = 0;
};

// 0x00E16BD4
class CClientBase : public Moho::IClient
{
public:
    Moho::IClientManager *mManager;
    int gap;
    Moho::BVIntSet mValidCommandSources;
    int mCommandSource;
    bool mReady;
    gpg::PipeStream mPipe;
    int mQueuedBeat;
    int mDispatchedBeat;
    int mAvailableBeatRemote;
    std::vector<int> mLatestAckReceived;
    int mLatestBeatDispatchedRemote;
    bool mEjectPending;
    bool mEjected;
    std::vector<Moho::SEjectRequest> mEjectRequests;
    int mSimRate;
};

// 0x00E16C1C
class CNullClient : public Moho::CClientBase
{};

// 0x00E16C64
class CLocalClient : public Moho::CClientBase
{};


// 0x00E16CA4
class CReplayClient : public Moho::CClientBase
{
public:
    gpg::PipeStream *mStrm;
    int v1;
    Moho::CMessage mMsg;
    int v23;
    int v24;
    bool v25;
    int v26;
    boost::condition mCondition;
    bool v33;
    bool v34;
};

class CNetClient :
    public Moho::CClientBase,     // 0x00E16CF4
    public Moho::IMessageReceiver // 0x00E16D38
{
public:
    Moho::INetConnection *mRemote;
};

}

