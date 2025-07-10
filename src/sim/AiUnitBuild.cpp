

bool Moho::CAiBuilderImpl::GetBool1() {
    return this->bool1;
}

void Moho::CAiBuilderImpl::SetBool1(bool val) {
    this->bool1 = val;
}

void Moho::CAiBuilderImpl::IssueRallyPoint() {

}

void Moho::CAiBuilderImpl::OnTick() {

}

bool Moho::CAiBuilderImpl::QueueIsEmpty() {
    return this->commands.start == nullptr || this->commands.finish - this->commands.start == 0;
}

bool Moho::CAiBuilderImpl::GetBool2() {
    return this->bool2;
}

void Moho::CAiBuilderImpl::SetBool2(bool val) {
    this->bool2 = val;
}

std::vector<Moho::WeakPtr<Moho::CUnitCommand>> Moho::CAiBuilderImpl::GetCommands() {
    return &this->commands;
}

bool Moho::CAiBuilderImpl::IsBusy() {

}

void Moho::CAiBuilderImpl::AddUnitToCommand(Moho::CUnitCommand *cmd, int comIndex) {
    cmd->AddUnit(&this->commands, this->unit, idx);
    this->bool2 = 1;
}

bool Moho::CAiBuilderImpl::HasCommand(Moho::CUnitCommand *cmd) {

}

void* Moho::CAiBuilderImpl::GetCommand(int idx) {

}

void Moho::CAiBuilderImpl::RemoveUnitToCommand(Moho::CUnitCommand *cmd) {

}

void Moho::CAiBuilderImpl::RemoveAllUnits() {

}

void Moho::CAiBuilderImpl::PrepareArmToBuild(Wm3::Vector3f) {

}

Wm3::Vector3f* Moho::CAiBuilderImpl::GetPos(Wm3::Vector3f *dest) {

}

void Moho::CAiBuilderImpl::SetBool3(bool val) {
    this->bool3 = val;
}

bool Moho::CAiBuilderImpl::GetBool3() {
    return this->bool3;
}

void Moho::CAiBuilderImpl::AddBuilding(Moho::SOCellPos *key, Moho::RUnitBlueprint *val) {

}

void Moho::CAiBuilderImpl::RemoveBuilding(Moho::SOCellPos *key) {

}

void Moho::CAiBuilderImpl::ClearBuildings() {

}

Moho::RUnitBlueprint* Moho::CAiBuilderImpl::GetFarthestBuilding(Moho::SOCellPos *dest) {

}
