#include "user/IClientManager.h"
#include "user/Marshaller.h"

namespace Moho {

// 0x00E16B64
class CClientManagerImpl : public Moho::IClientManager
{
public:
    boost::recursive_mutex mLock;
    Moho::IClientMgrUIInterface *mInterface;
    std::vector<Moho::CClientBase*> mClients;
    Moho::INetConnector *mConnector;
    Moho::CClientBase *mLocalClient;
    bool mWeAreReady;
    bool mEveryoneIsReady;
    int mDispatchedBeat;
    int mAvailableBeat;
    int mFullyQueuedBeat;
    int mPartiallyQueuedBeat;
    int mGameSpeedClock;
    int mGameSpeedRequester;
    int mGameSpeed;
    bool mAdjustableGameSpeed;
    HANDLE mCurrentEvent;
    int gap;
    gpg::time::Timer mTimer3;
    struct_CyclicBuffer<struct_a2, 4096> mBuff;
    gpg::PipeStream mStream;
    Moho::CMarshaller mMarshaller;
    gpg::time::Timer mDispatchedTimer;
    gpg::time::Timer mTimer2;

    ~CClientManagerImpl() = default; // 0x0053E050
    Moho::IClient *CreateLocalClient(const char *name, int idx, Moho::LaunchInfoBase *info, unsigned int sourceId) override; // 0x0053E180
    Moho::IClient *CreateNetClient(const char *name, int idx, Moho::LaunchInfoBase *info, unsigned int sourceId, int val) override; // 0x0053E260
    Moho::IClient *CreateReplayClient(int *, Moho::BVIntSet *) override; // 0x0053E400
    Moho::IClient *CreateNullClient(const char *name, int idx, Moho::LaunchInfoBase *info, unsigned int sourceId) override; // 0x0053E330
    Moho::INetConnector *GetConnector() override; // 0x0053BCB0
    size_t NumberOfClients() override; // 0x0053BCC0
    Moho::IClient *GetClient(int idx) override; // 0x0053BCE0
    Moho::IClient *GetClientWithData(Moho::LaunchInfoBase *info) override; // 0x0053E470
    Moho::IClient *GetLocalClient() override; // 0x0053BD10
    void SetUIInterface(Moho::IClientMgrUIInterface *) override; // 0x0053BD20
    void Cleanup() override; // 0x0053E4B0
    bool IsEveryoneReady() override; // 0x0053E560
    void SetSimRate(int rate) override; // 0x0053E590
    int GetSimRate() override; // 0x0053E720
    int GetSimRateRequested() override; // 0x0053E7E0
    void Func1(int) override; // 0x0053E850
    void ProcessClients(Moho::CMessage &msg) override; // 0x0053E990
    void DoBeat() override; // 0x0053EA30
    void SelectEvent(HANDLE ev) override; // 0x0053EDA0
    int &GetPartiallyQueuedBeat(int &out) override; // 0x0053EF90
    int &GetAvailableBeat(int &out) override; // 0x0053EFD0
    void UpdateStates(int beat) override; // 0x0053F010
    struct_a3 GetBetween(int since) override; // 0x0053F4C0
    Moho::SClientBottleneckInfo GetBottleneckInfo() override; // 0x0053F5A0
    void Debug() override; // 0x0053F920
    void Disconnect() override; // 0x0053F830
};

Moho::IClientManager *CLIENT_CreateClientManager(size_t numClients, Moho::INetConnector *conn, int speed, bool speedAdjustable); // 0x0053FAF0

}
