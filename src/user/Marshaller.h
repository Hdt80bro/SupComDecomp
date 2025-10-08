#include "user/IClientManager.h"
#include "core/CommandSink.h"
#include "gpgcore/streams/BinaryWriter.h"

namespace Moho {

// 0x00E2E7FC
class CMarshaller : Moho::ICommandSink
{
public:
    Moho::CClientManagerImpl *mClientManager;

    void SetCommandSource(unsigned int) override; // 0x006E5A90
    void CommandSourceTerminated() override; // 0x006E5B90
    void VerifyChecksum(const gpg::MD5Digest &, Moho::CSeqNo) override; // 0x006E5C70
    void RequestPause() override; // 0x006E5DB0
    void Resume() override; // 0x006E5E90
    void SingleStep() override; // 0x006E5F70
    void CreateUnit(unsigned int, const Moho::RResId &, const Moho::SCoordsVec2 &, float) override; // 0x006E6050
    void CreateProp(gpg::StrArg, const Wm3::Vector3f &) override; // 0x006E61E0
    void DestroyEntity(Moho::EntId) override; // 0x006E6320
    void WarpEntity(Moho::EntId, const Moho::VTransform &) override; // 0x006E6420
    void ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) override; // 0x006E6560
    void IssueCommand(const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) override; // 0x006E6690
    void IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &, const Moho::SSTICommandIssueData &, bool) override; // 0x006E67D0
    void IncreaseCommandCount(Moho::CmdId, int) override; // 0x006E6910
    void DecreaseCommandCount(Moho::CmdId, int) override; // 0x006E6A40
    void SetCommandTarget(Moho::CmdId, const Moho::SSTITarget &) override; // 0x006E6B70
    void SetCommandType(Moho::CmdId, Moho::EUnitCommandType) override; // 0x006E6C90
    void SetCommandCells(Moho::CmdId, const gpg::fastvector<Moho::SOCellPos> &, const Wm3::Vector3f &) override; // 0x006E6DD0
    void RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) override; // 0x006E6F30
    void ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) override; // 0x006E71F0
    void LuaSimCallback(gpg::StrArg, const LuaPlus::LuaObject &, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) override; // 0x006E7300
    void ExecuteDebugCommand(gpg::StrArg, const Wm3::Vector3f &, unsigned int, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &) override; // 0x006E7070
    void AdvanceBeat(int) override; // 0x006E7420
    void EndGame() override; // 0x006E7520

    void WriteEntIdSet(gpg::BinaryWriter &, const Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> &); // 0x006E7600
    void WriteCommandData(gpg::BinaryWriter &, const Moho::SSTICommandIssueData &); // 0x006E76C0
    void WriteTarget(gpg::BinaryWriter &, const Moho::SSTITarget &); // 0x006E7890
    void WriteCells(gpg::BinaryWriter &, const gpg::fastvector<Moho::SOCellPos> &); // 0x006E7940
};

    
}