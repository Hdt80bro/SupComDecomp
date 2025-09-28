#include "core/Sync.h"
#include "user/Camera.h"
#include "user/ClientManager.h"
#include <memory>

namespace Moho {

// 0x00E33464
class ISTIDriver
{
public:
    virtual ~ISTIDriver() = default; // 0x0073B0D0
    virtual void DisconnectClients() = 0;
    virtual void Shutdown() = 0;
    virtual Moho::CClientManagerImpl *GetClientManager() = 0;
    virtual void Func1() = 0;
    virtual void Dispatch() = 0;
    virtual void IncVar1() = 0;
    virtual void DecTime1() = 0;
    virtual bool HasSyncData() = 0;
    virtual std::auto_ptr<Moho::SSyncData> GetSyncData() = 0;
    virtual HANDLE GetEvent1() = 0;
    virtual float GetSimSpeed() = 0;
    virtual void SetArmyIndex(int idx) = 0;
    virtual void SetGeomCams(std::vector<Moho::GeomCamera3> &cams) = 0;
    virtual void TryFilter(Moho::EntityCategory) = 0;
    virtual void SetFilter(Moho::EntityCategory) = 0;
    virtual void SetBool1(bool) = 0;
    virtual int RequestPause() = 0;
    virtual int Resume() = 0;
    virtual int SingleStep() = 0;
    virtual int CreateUnit(unsigned int, const Moho::RResId &, const Moho::SCoordsVec2 &, float) = 0;
    virtual int CreateProp(gpg::StrArg, const Wm3::Vector3f &) = 0;
    virtual int DestroyEntity(Moho::EntId) = 0;
    virtual int WarpEntity(Moho::EntId, const Moho::VTransform &) = 0;
    virtual int ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) = 0;
    virtual int IssueCommand(const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) = 0;
    virtual int IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) = 0;
    virtual int IncreaseCommandCount(Moho::CmdId, int) = 0;
    virtual int DecreaseCommandCount(Moho::CmdId, int) = 0;
    virtual int SetCommandTarget(Moho::CmdId, const Moho::SSTITarget &) = 0;
    virtual int SetCommandType(Moho::CmdId, Moho::EUnitCommandType) = 0;
    virtual int SetCommandCells(Moho::CmdId, const gpg::fastvector<Moho::SOCellPos> &, const Wm3::Vector3f &) = 0;
    virtual int RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) = 0;
    virtual int ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) = 0;
    virtual int LuaSimCallback(gpg::StrArg, const LuaPlus::LuaObject &, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) = 0;
    virtual int ExecuteDebugCommand(gpg::StrArg, const Wm3::Vector3f &, unsigned int, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) = 0;
    virtual void ProcessEvents() = 0;
    virtual void DecVar3() = 0;
    virtual void RequestSaveGame(Moho::CSaveGameRequestImpl *) = 0;
    virtual void DrawNetworkStats(Moho::CD3DPrimbatcher &batcher, float x, float y, float w, float h) = 0;
};

// 0x00E3350C
class CSimDriver : public Moho::ISTIDriver
{
public:

    ~CSimDriver() override; // 0x0073B910
    void DisconnectClients() override; // 0x0073BBF0
    void Shutdown() override; // 0x0073BC80
    Moho::CClientManagerImpl *GetClientManager() override; // 0x0073B190
    void Func1() override; // 0x0073BDE0
    void Dispatch() override; // 0x0073C250
    void IncVar1() override; // 0x0073C410
    void DecTime1() override; // 0x0073C440
    bool HasSyncData() override; // 0x0073C4F0
    std::auto_ptr<Moho::SSyncData> GetSyncData() override; // 0x0073C520
    HANDLE GetEvent1() override; // 0x0073B1A0
    float GetSimSpeed() override; // 0x0073C630
    void SetArmyIndex(int idx) override; // 0x0073B1B0
    void SetGeomCams(std::vector<Moho::GeomCamera3> &cams) override; // 0x0073B270
    void TryFilter(Moho::EntityCategory) override; // 0x0073B3F0
    void SetFilter(Moho::EntityCategory) override; // 0x0073B4B0
    void SetBool1(bool) override; // 0x0073B240
    int RequestPause() override; // 0x0073C660
    int Resume() override; // 0x0073C700
    int SingleStep() override; // 0x0073C7A0
    int CreateUnit(unsigned int, const Moho::RResId &, const Moho::SCoordsVec2 &, float) override; // 0x0073C840
    int CreateProp(gpg::StrArg, const Wm3::Vector3f &) override; // 0x0073C8F0
    int DestroyEntity(Moho::EntId) override; // 0x0073C990
    int WarpEntity(Moho::EntId, const Moho::VTransform &) override; // 0x0073CA30
    int ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) override; // 0x0073CAD0
    int IssueCommand(const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) override; // 0x0073CB70
    int IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) override; // 0x0073CC10
    int IncreaseCommandCount(Moho::CmdId, int) override; // 0x0073CCB0
    int DecreaseCommandCount(Moho::CmdId, int) override; // 0x0073CD50
    int SetCommandTarget(Moho::CmdId, const Moho::SSTITarget &) override; // 0x0073CDF0
    int SetCommandType(Moho::CmdId, Moho::EUnitCommandType) override; // 0x0073CE90
    int SetCommandCells(Moho::CmdId, const gpg::fastvector<Moho::SOCellPos> &, const Wm3::Vector3f &) override; // 0x0073CF30
    int RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) override; // 0x0073CFD0
    int ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) override; // 0x0073D070
    int LuaSimCallback(gpg::StrArg, const LuaPlus::LuaObject &, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) override; // 0x0073D110
    int ExecuteDebugCommand(gpg::StrArg, const Wm3::Vector3f &, unsigned int, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) override; // 0x0073D1B0
    void ProcessEvents() override; // 0x0073DEA0
    void DecVar3() override; // 0x0073DF50
    void RequestSaveGame(Moho::CSaveGameRequestImpl *) override; // 0x0073DF60
    void DrawNetworkStats(Moho::CD3DPrimbatcher &batcher, float x, float y, float w, float h) override; // 0x0073DFE0
};

Moho::ISTIDriver *SIM_CreateDriver(std::auto_ptr<Moho::IClientManager>, std::auto_ptr<gpg::Stream>, const boost::shared_ptr<Moho::LaunchInfoBase> &, unsigned int); // 0x0073F4E0

}
