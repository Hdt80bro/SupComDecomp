#include "gpgcore/MD5.h"
#include "gpgcore/String.h"

namespace Moho {

using CSeqNo = unsigned int;
using CmdId = unsigned int;

// 0x00E2E794
class ICommandSink
{
public:
    virtual void SetCommandSource(unsigned int) = 0;
    virtual void CommandSourceTerminated() = 0;
    virtual void VerifyChecksum(const gpg::MD5Digest &, Moho::CSeqNo) = 0;
    virtual void RequestPause() = 0;
    virtual void Resume() = 0;
    virtual void SingleStep() = 0;
    virtual void CreateUnit(unsigned int, const Moho::RResId &, const Moho::SCoordsVec2 &, float) = 0;
    virtual void CreateProp(gpg::StrArg, const Wm3::Vector3f &) = 0;
    virtual void DestroyEntity(Moho::EntId) = 0;
    virtual void WarpEntity(Moho::EntId, const Moho::VTransform &) = 0;
    virtual void ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) = 0;
    virtual void IssueCommand(const Moho::EntityCategory &, const Moho::SSTICommandIssueData &, bool) = 0;
    virtual void IssueFactoryCommand(Moho::EntityCategory &, const Moho::SSTICommandIssueData &, bool) = 0;
    virtual void IncreaseCommandCount(Moho::CmdId, int) = 0;
    virtual void DecreaseCommandCount(Moho::CmdId, int) = 0;
    virtual void SetCommandTarget(Moho::CmdId, const Moho::SSTITarget &) = 0;
    virtual void SetCommandType(Moho::CmdId, Moho::EUnitCommandType) = 0;
    virtual void SetCommandCells(Moho::CmdId, const gpg::fastvector<Moho::SOCellPos> &, const Wm3::Vector3f &) = 0;
    virtual void RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) = 0;
    virtual void ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) = 0;
    virtual void LuaSimCallback(gpg::StrArg, const LuaPlus::LuaObject &, const Moho::EntityCategory &) = 0;
    virtual void ExecuteDebugCommand(gpg::StrArg, const Wm3::Vector3f &, unsigned int, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) = 0;
    virtual void AdvanceBeat(int) = 0;
    virtual void EndGame();
};
    
}