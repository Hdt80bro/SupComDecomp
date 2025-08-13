// known file

#include "AiUnitBuild.h"

// 0x0059FAA0
bool Moho::CAiBuilderImpl::GetBool1() {
    return this->bool1;
}

// 0x0059FA90
void Moho::CAiBuilderImpl::SetBool1(bool val) {
    this->bool1 = val;
}

// 0x0059EEF0
void Moho::CAiBuilderImpl::IssueRallyPoint() {

}

// 0x0059F220
void Moho::CAiBuilderImpl::OnTick() {

}

// 0x0059F440
bool Moho::CAiBuilderImpl::QueueIsEmpty() {
    return this->commands.start == nullptr || this->commands.finish - this->commands.start == 0;
}

// 0x0059EED0
bool Moho::CAiBuilderImpl::GetBool2() {
    return this->bool2;
}

// 0x0059EEE0
void Moho::CAiBuilderImpl::SetBool2(bool val) {
    this->bool2 = val;
}

// 0x0059F470
std::vector<Moho::WeakPtr<Moho::CUnitCommand>> Moho::CAiBuilderImpl::GetCommands() {
    return &this->commands;
}

// 0x0059F480
bool Moho::CAiBuilderImpl::IsBusy() {

}

// 0x0059F4D0
void Moho::CAiBuilderImpl::AddUnitToCommand(Moho::CUnitCommand *cmd, int comIndex) {
    cmd->AddUnit(&this->commands, this->unit, idx);
    this->bool2 = 1;
}

// 0x0059F500
bool Moho::CAiBuilderImpl::HasCommand(Moho::CUnitCommand *cmd) {

}

// 0x0059F540
void* Moho::CAiBuilderImpl::GetCommand(int idx) {

}

// 0059F580
void Moho::CAiBuilderImpl::RemoveUnitToCommand(Moho::CUnitCommand *cmd) {

}

// 0x0059F5A0
void Moho::CAiBuilderImpl::RemoveAllUnits() {

}

// 0x0059F600
void Moho::CAiBuilderImpl::PrepareArmToBuild(Wm3::Vector3f) {

}

// 0x0059F650
Wm3::Vector3f* Moho::CAiBuilderImpl::GetPos(Wm3::Vector3f *dest) {

}

// 0x0059F670
void Moho::CAiBuilderImpl::SetBool3(bool val) {
    this->bool3 = val;
}

// 0x0059F680
bool Moho::CAiBuilderImpl::GetBool3() {
    return this->bool3;
}

// 0x0059F690
void Moho::CAiBuilderImpl::AddBuilding(Moho::SOCellPos *key, Moho::RUnitBlueprint *val) {

}

// 0x0059F6C0
void Moho::CAiBuilderImpl::RemoveBuilding(Moho::SOCellPos *key) {

}

// 0x0059F710
void Moho::CAiBuilderImpl::ClearBuildings() {

}

// 0x0059F740
Moho::RUnitBlueprint* Moho::CAiBuilderImpl::GetFarthestBuilding(Moho::SOCellPos *dest) {

}
