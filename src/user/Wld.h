
#include "user/WldMap.h"
#include "user/WldProps.h"
#include "user/WldSession.h"
#include "user/WldTerrain.h"
#include "user/WldUiInterface.h"
#include "user/WldUIProvider.h"

#include "user/SimDriver.h"
#include "LuaPlus.h"
#include <memory>
#include <string>


enum enum_WldFrameAction
{
    FRAME_ACTION_INACTIVE = 0x0,
    FRAME_ACTION_PRELOAD = 0x1,
    FRAME_ACTION_LOADING = 0x2,
    FRAME_ACTION_INITIALIZE = 0x3,
    FRAME_ACTION_4 = 0x4,
    FRAME_ACTION_5 = 0x5,
    FRAME_ACTION_PLAYING = 0x6,
    FRAME_ACTION_CREATE_SESSION = 0x7,
    FRAME_ACTION_EXIT = 0x8,
};


static Moho::IWldUIProvider *sWldUIProvider; // 0x010A6468
static enum_WldFrameAction sWldFrameAction; // 0x010A646C
static Moho::CWldSession *sWldSession; // 0x010A6470
static Moho::ISTIDriver *sSimDriver; // 0x010C4F50

namespace Moho {

void WLD_SetUIProvider(Moho::IWldUIProvider *prov); // 0x0088BD20
LuaPlus::LuaObject WLD_LoadScenarioInfo(const std::string &scen, LuaPlus::LuaState *state); // 0x0088BD40
bool WLD_IsSessionActive(); // 0x0088BE80
void WLD_RequestEndSession(); // 0x0088BEA0
void WLD_RequestRestartSession(); // 0x0088BEC0
void WLD_Teardown(); // 0x0088C860
bool WLD_Frame(float delta); // 0x0088CAE0
std::auto_ptr<Moho::SWldSessionInfo> WLD_SetupSessionInfo(const LuaPlus::LuaObject &); // 0x0088CBC0
void WLD_BeginSession(std::auto_ptr<Moho::SWldSessionInfo>); // 0x0088D060
float WLD_GetSimRate(); // 0x0088D0B0
void WLD_IncreaseSimRate(); // 0x0088D1B0
void WLD_ResetSimRate(); // 0x0088D220
void WLD_DecreaseSimRate(); // 0x0088D280
void WLD_SetGameSpeed(int rate); // 0x0088D2F0
Moho::ISTIDriver *WLD_GetDriver(); // 0x0088D330
std::auto_ptr<Moho::CWldMap> WLD_LoadMapPreview(gpg::StrArg); // 0x00891330
Moho::CWldProps *WLD_CreateProps(); // 0x00892210
Moho::CWldSession *WLD_CreateSession(std::auto_ptr<LuaPlus::LuaState> &state, std::auto_ptr<Moho::RRuleGameRules> &rules, std::auto_ptr<Moho::CWldMap> &map, Moho::SWldSessionInfo &info); // 0x00897220
void WLD_DestroySession(); // 0x008972A0
Moho::CWldSession *WLD_GetSession(); // 0x008972F0
Moho::IWldTerrainRes *WLD_CreateTerrainRes(); // 0x008A7B90

}
