#include "Sim.h"

// 0x00748650
void Moho::Sim::SetCommandSource() {

}

// 0x007486B0
void Moho::Sim::CommandSourceTerminated() {

}

// 0x007487C0
void Moho::Sim::VerifyChecksum(gpg::MD5Digest const &, Moho::CSeqNo) {

}

// 0x00748960
void Moho::Sim::RequestPause() {

}

// 0x007489A0
void Moho::Sim::Resume() {

}

// 0x007489C0
void Moho::Sim::SingleStep() {

}

// 0x00748AA0
void Moho::Sim::CreateUnit(uint, Moho::RResId const &, Moho::SCoordsVec2 const &, float) {

}

// 0x00748C00
void Moho::Sim::CreateProp(gpg::StrArg, Wm3::Vector3f const &) {

}

// 0x00748C80
void Moho::Sim::DestroyEntity(Moho::EntId) {

}

// 0x00748CD0
void Moho::Sim::WarpEntity(Moho::EntId, Moho::VTransform const &) {

}

// 0x00748D50
void Moho::Sim::ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) {

}

// 0x00749290
void Moho::Sim::IssueCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &, Moho::SSTICommandIssueData const &, bool) {

}

// 0x007494B0
void Moho::Sim::IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &, Moho::SSTICommandIssueData const &, bool) {

}

// 0x00749680
void Moho::Sim::IncreaseCommandCount(Moho::CmdId, int) {

}

// 0x007496E0
void Moho::Sim::DecreaseCommandCount(Moho::CmdId, int) {

}

// 0x00749740
void Moho::Sim::SetCommandTarget(Moho::CmdId, Moho::SSTITarget const &) {

}

// 0x00749800
void Moho::Sim::SetCommandType(Moho::CmdId, Moho::EUnitCommandType) {

}

// 0x00749860
void Moho::Sim::SetCommandCells(Moho::CmdId, gpg::fastvector<Moho::SOCellPos> const &, Wm3::Vector3f const &) {

}

// 0x00749970
void Moho::Sim::RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) {

}

// 0x00749A70
void Moho::Sim::ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) {

}

// 0x00749B60
void Moho::Sim::LuaSimCallback(gpg::StrArg, LuaPlus::LuaObject const &, Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &) {

}

// 0x00749DA0
void Moho::Sim::ExecuteDebugCommand(gpg::StrArg, Wm3::Vector3f const &, uint, Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &) {

}

// 0x00749F40
void Moho::Sim::AdvanceBeat(int) {

}

// 0x0074B100
void Moho::Sim::EndGame() {
    this->gameEnded = true;
}
