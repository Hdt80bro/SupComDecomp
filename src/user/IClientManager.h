#include "user/Client.h"
#include "user/LaunchInfo.h"
#include "user/WldUiInterface.h"
#include "core/Message.h"

// 0x00E16AF4
class IClientManager : public Moho::CMessageDispatcher
{
public:
    virtual ~IClientManager() = default; // 0x0053B680
    virtual Moho::IClient *CreateLocalClient(const char *name, int idx, Moho::LaunchInfoBase *info, unsigned int sourceId) = 0;
    virtual Moho::IClient *CreateNetClient(const char *name, int idx, Moho::LaunchInfoBase *info, unsigned int sourceId, int val) = 0;
    virtual Moho::IClient *CreateReplayClient(int *, Moho::BVIntSet *) = 0;
    virtual Moho::IClient *CreateNullClient(const char *name, int idx, Moho::LaunchInfoBase *info, unsigned int sourceId) = 0;
    virtual Moho::INetConnector *GetConnector() = 0;
    virtual size_t NumberOfClients() = 0;
    virtual Moho::IClient *GetClient(int idx) = 0;
    virtual Moho::IClient *GetClientWithData(Moho::LaunchInfoBase *info) = 0;
    virtual Moho::IClient *GetLocalClient() = 0;
    virtual void SetUIInterface(Moho::IClientMgrUIInterface *) = 0;
    virtual void Cleanup() = 0;
    virtual bool IsEveryoneReady() = 0;
    virtual void SetSimRate(int rate) = 0;
    virtual int GetSimRate() = 0;
    virtual int GetSimRateRequested() = 0;
    virtual void Func1(int) = 0;
    virtual void ProcessClients(Moho::CMessage &msg) = 0;
    virtual void DoBeat() = 0;
    virtual void SelectEvent(HANDLE ev) = 0;
    virtual int &GetPartiallyQueuedBeat(int &out) = 0;
    virtual int &GetAvailableBeat(int &out) = 0;
    virtual void UpdateStates(int beat) = 0;
    virtual struct_a3 GetBetween(int since) = 0;
    virtual Moho::SClientBottleneckInfo GetBottleneckInfo() = 0;
    virtual void Debug() = 0;
    virtual void Disconnect() = 0;
};
