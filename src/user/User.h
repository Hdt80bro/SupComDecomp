#include <string>
#include "LuaPlus.h"

namespace Moho {

enum ESpecialFileType
{
    SFT_SaveGame = 0x0,
    SFT_Replay = 0x1,
    SFT_CampaignSave = 0x3,
};


Moho::IUserSoundManager *USER_GetSound(); // 0x008AB220
LuaPlus::LuaState *Moho::USER_GetLuaState(); // 0x008C65B0
void USER_LuaFrame(); // 0x008C6730
void USER_SetCompanyName(const char *); // 0x008C8640
void USER_SetAppName(const char *); // 0x008C8670
void USER_SetAppExtensionPrefix(const char *); // 0x008C86A0
void USER_SetGameId(const _GUID&); // 0x008C86D0
void USER_SetCurrentProfile(const std::string &); // 0x008C8760
void USER_LoadPreferences(const std::string &); // 0x008C89D0
Moho::IUserPrefs *USER_GetPreferences(); // 0x008C9110
void USER_SavePreferences(); // 0x008C91A0
std::string USER_GetAppDataDir(); // 0x008C9D10
std::string USER_GetAppDocDir(); // 0x008C9E20
std::string USER_GetAppCacheDir(); // 0x008C9F90
void USER_PurgeAppCacheDir(); // 0x008CA070
std::string USER_GetSaveGameDir(bool); // 0x008CA2D0
std::string USER_GetSaveGameExt(); // 0x008CA380
std::string USER_GetReplayDir(bool); // 0x008CA3A0
std::string USER_GetReplayExt(); // 0x008CA450
std::string USER_GetCampaignSaveExt(); // 0x008CA470
std::string USER_GetScreenshotDir(); // 0x008CA490
void USER_GetSpecialFiles(Moho::ESpecialFileType, std::string &, std::string &, std::map<std::string, std::vector<std::string>> &); // 0x008CA650
std::string USER_GetAppName(); // 0x008CAE00
_GUID const &USER_GetGameId(); // 0x008CAE10
bool USER_DebugFacilitiesEnabled(); // 0x008CAE20

}
