#include <gpgcore/String.h>
#include "LuaPlus.h"

namespace Moho {

int OPTIONS_GetInt(gpg::StrArg); // 0x008C68A0
float OPTIONS_GetFloat(gpg::StrArg); // 0x008C69A0
std::string OPTIONS_GetString(gpg::StrArg); // 0x008C6AB0
bool OPTIONS_GetBool(gpg::StrArg); // 0x008C6BF0
void OPTIONS_Apply(); // 0x008C6CF0
void OPTIONS_SetCustomData(gpg::StrArg, const LuaPlus::LuaObject &, const LuaPlus::LuaObject &); // 0x008C6DC0
void OPTIONS_CreateInitialProfileIfNeeded(gpg::StrArg); // 0x008C6EC0
std::string OPTIONS_GetCurrentProfileName(); // 0x008C7040

}