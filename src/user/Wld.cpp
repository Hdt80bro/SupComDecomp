#include "Wld.h"


Moho::IWldUIProvider *sWldUIProvider; // 0x010A6468
enum_WldFrameAction sWldFrameAction; // 0x010A646C
Moho::CWldSession *sWldSession; // 0x010A6470
Moho::ISTIDriver *sSimDriver; // 0x010C4F50


// 0x0088BD20
void Moho::WLD_SetUIProvider(Moho::IWldUIProvider *prov) {
    if (sWldUIProvider != nullptr && sWldUIProvider != prov) {
        delete(sWldUIProvider);
    }
    wlduiprovider = prov;
}

// 0x0088BD40
LuaPlus::LuaObject Moho::WLD_LoadScenarioInfo(const std::string &scen, LuaPlus::LuaState *state) {
    LuaPlus::LuaObject scenObj{state};
    // ...
}

// 0x0088BE80
bool Moho::WLD_IsSessionActive() {
    return sWldFrameAction != FRAME_ACTION_INACTIVE;
}

// 0x0088BEA0
void Moho::WLD_RequestEndSession() {
    if (sWldFrameAction != FRAME_ACTION_INACTIVE) {
        sWldFrameAction = FRAME_ACTION_EXIT;
    }
}

// 0x0088BEC0
void Moho::WLD_RequestRestartSession() {
    if (sWldFrameAction != FRAME_ACTION_INACTIVE &&
        sWldSession->canRestartvar
    ) {
        sWldFrameAction = FRAME_ACTION_CREATE_SESSION;
    }
}

// 0x0088C860
void Moho::WLD_Teardown() {
    Moho::ISTIDriver *driver = Moho::WLD_GetDriver();
    if (driver != nullptr) {

    }
}

// 0x0088CAE0
bool Moho::WLD_Frame(float delta) {

}

// 0x0088CBC0
std::auto_ptr<Moho::SWldSessionInfo> Moho::WLD_SetupSessionInfo(const LuaPlus::LuaObject &) {

}

// 0x0088D060
void Moho::WLD_BeginSession(std::auto_ptr<Moho::SWldSessionInfo>) {

}

// 0x0088D0B0
float Moho::WLD_GetSimRate() {

}

// 0x0088D1B0
void Moho::WLD_IncreaseSimRate() {

}

// 0x0088D220
void Moho::WLD_ResetSimRate() {

}

// 0x0088D280
void Moho::WLD_DecreaseSimRate() {

}

// 0x0088D2F0
void Moho::WLD_SetGameSpeed(int rate) {

}

// 0x0088D330
Moho::ISTIDriver *Moho::WLD_GetDriver() {

}

// 0x00891330
std::auto_ptr<Moho::CWldMap> Moho::WLD_LoadMapPreview(gpg::StrArg) {

}

// 0x00892210
Moho::CWldProps *Moho::WLD_CreateProps() {

}

// 0x00897220
Moho::CWldSession *Moho::WLD_CreateSession(std::auto_ptr<LuaPlus::LuaState> &state, std::auto_ptr<Moho::RRuleGameRules> &rules, std::auto_ptr<Moho::CWldMap> &map, Moho::SWldSessionInfo &info) {

}

// 0x008972A0
void Moho::WLD_DestroySession() {

}

// 0x008972F0
Moho::CWldSession *Moho::WLD_GetSession() {

}

// 0x008A7B90
Moho::IWldTerrainRes *Moho::WLD_CreateTerrainRes() {

}
