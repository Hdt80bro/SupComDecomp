void Moho::Sim::SetCommandSource() {

}

void Moho::Sim::CommandSourceTerminated() {

}

void Moho::Sim::VerifyChecksum(gpg::MD5Digest const &, Moho::CSeqNo) {

}

void Moho::Sim::RequestPause() {

}

void Moho::Sim::Resume() {

}

void Moho::Sim::SingleStep() {

}

void Moho::Sim::CreateUnit(uint, Moho::RResId const &, Moho::SCoordsVec2 const &, float) {

}

void Moho::Sim::CreateProp(gpg::StrArg, Wm3::Vector3f const &) {

}

void Moho::Sim::DestroyEntity(Moho::EntId) {

}

void Moho::Sim::WarpEntity(Moho::EntId, Moho::VTransform const &) {

}

void Moho::Sim::ProcessInfoPair(Moho::EntId, gpg::StrArg, gpg::StrArg) {

}

void Moho::Sim::IssueCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &, Moho::SSTICommandIssueData const &, bool) {

}

void Moho::Sim::IssueFactoryCommand(Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &, Moho::SSTICommandIssueData const &, bool) {

}

void Moho::Sim::IncreaseCommandCount(Moho::CmdId, int) {

}

void Moho::Sim::DecreaseCommandCount(Moho::CmdId, int) {

}

void Moho::Sim::SetCommandTarget(Moho::CmdId, Moho::SSTITarget const &) {

}

void Moho::Sim::SetCommandType(Moho::CmdId, Moho::EUnitCommandType) {

}

void Moho::Sim::SetCommandCells(Moho::CmdId, gpg::fastvector<Moho::SOCellPos> const &, Wm3::Vector3f const &) {

}

void Moho::Sim::RemoveCommandFromUnitQueue(Moho::CmdId, Moho::EntId) {

}

void Moho::Sim::ExecuteLuaInSim(gpg::StrArg, LuaPlus::LuaObject const &) {

}

void Moho::Sim::LuaSimCallback(gpg::StrArg, LuaPlus::LuaObject const &, Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &) {

}

void Moho::Sim::ExecuteDebugCommand(gpg::StrArg, Wm3::Vector3f const &, uint, Moho::BVSet<Moho::EntId, Moho::EntIdUniverse> const &) {

}

void Moho::Sim::AdvanceBeat(int) {

}

void Moho::Sim::EndGame() {
    this->gameEnded = 1;
}
