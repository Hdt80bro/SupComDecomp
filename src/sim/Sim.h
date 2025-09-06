#include "core/CommandSink.h"
#include "gpgcore/String.h"
#include "gpgcore/MD5.h"
#include "gpgcore/containers/fastvector.h"
#include "LuaPlus.h"
#include "Wm3Vector3.h"
#include "boost/shared_ptr.hpp"

namespace Moho {

// 0x00E34714
class Sim : public Moho::ICommandSink
{
public:
    static Moho::Sim sInstance;

    static Moho::Sim * Create(boost::shared_ptr<Moho::LaunchInfoBase> const &);

public:

    void SetCommandSource(unsigned int) override; // 0x00748650
    void CommandSourceTerminated() override; // 0x007486B0
    void VerifyChecksum(const gpg::MD5Digest &, Moho::CSeqNo) override; // 0x007487C0
    void RequestPause() override; // 0x00748960
    void Resume() override; // 0x007489A0
    void SingleStep() override; // 0x007489C0
    void CreateUnit(unsigned int, const Moho::RResId &, const Moho::SCoordsVec2 &, float) override; // 0x00748AA0
    void CreateProp(gpg::StrArg, const Wm3::Vector3f &) override; // 0x00748C00
    void DestroyEntity(Moho::EntId) override; // 0x00748C80
    void WarpEntity(Moho::EntId, const Moho::VTransform &) override; // 0x00748CD0
    void ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) override; // 0x00748D50
    void IssueCommand(const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) override; // 0x00749290
    void IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) override; // 0x007494B0
    void IncreaseCommandCount(Moho::CmdId, int) override; // 0x00749680
    void DecreaseCommandCount(Moho::CmdId, int) override; // 0x007496E0
    void SetCommandTarget(Moho::CmdId, const Moho::SSTITarget &) override; // 0x00749740
    void SetCommandType(Moho::CmdId, Moho::EUnitCommandType) override; // 0x00749800
    void SetCommandCells(Moho::CmdId, const gpg::fastvector<Moho::SOCellPos> &, const Wm3::Vector3f &) override; // 0x00749860
    void RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) override; // 0x00749970
    void ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) override; // 0x00749A70
    void LuaSimCallback(gpg::StrArg, const LuaPlus::LuaObject &, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) override; // 0x00749B60
    void ExecuteDebugCommand(gpg::StrArg, const Wm3::Vector3f &, unsigned int, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) override; // 0x00749DA0
    void AdvanceBeat(int) override; // 0x00749F40
    void EndGame() override; // 0x0074B100

    Sim(Moho::LaunchInfoBase *info); // 0x007434D0
    void Setup(Moho::LaunchInfoBase *info); // 0x00744060
    void CreateArmies(const std::vector<Moho::ArmyLaunchInfo> &, const std::vector<LuaPlus::LuaObject> &, const LuaPlus::LuaObject &); // 0x00746310
    void PostInitialize(const LuaPlus::LuaObject &); // 0x007464D0
    bool CheatsEnabled(); // 0x00747180
    bool OkayToMessWith(Moho::SimArmy *); // 0x00747360
};

}
