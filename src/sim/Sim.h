
namespace Moho {

class Sim
{

    virtual void SetCommandSource(); // 0x00748650
    virtual void CommandSourceTerminated(); // 0x007486B0
    virtual void VerifyChecksum(gpg::MD5Digest const &, Moho::CSeqNo); // 0x007487C0
    virtual void RequestPause(); // 0x00748960
    virtual void Resume(); // 0x007489A0
    virtual void SingleStep(); // 0x007489C0
    virtual void CreateUnit(unsigned int, Moho::RResId const &, Moho::SCoordsVec2 const &, float); // 0x00748AA0
    virtual void CreateProp(gpg::StrArg, Wm3::Vector3f const &); // 0x00748C00
    virtual void DestroyEntity(Moho::EntId); // 0x00748C80
    virtual void WarpEntity(Moho::EntId, Moho::VTransform const &); // 0x00748CD0
    virtual void ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg); // 0x00748D50
    virtual void IssueCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &, Moho::SSTICommandIssueData const &, bool); // 0x00749290
    virtual void IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &, Moho::SSTICommandIssueData const &, bool); // 0x007494B0
    virtual void IncreaseCommandCount(Moho::CmdId, int); // 0x00749680
    virtual void DecreaseCommandCount(Moho::CmdId, int); // 0x007496E0
    virtual void SetCommandTarget(Moho::CmdId, Moho::SSTITarget const &); // 0x00749740
    virtual void SetCommandType(Moho::CmdId, Moho::EUnitCommandType); // 0x00749800
    virtual void SetCommandCells(Moho::CmdId, gpg::fastvector<Moho::SOCellPos> const &, Wm3::Vector3f const &); // 0x00749860
    virtual void RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId); // 0x00749970
    virtual void ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &); // 0x00749A70
    virtual void LuaSimCallback(gpg::StrArg, LuaPlus::LuaObject const &, Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &); // 0x00749B60
    virtual void ExecuteDebugCommand(gpg::StrArg, Wm3::Vector3f const &, uint, Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &); // 0x00749DA0
    virtual void AdvanceBeat(int); // 0x00749F40
    virtual void EndGame(); // 0x0074B100

};

}
