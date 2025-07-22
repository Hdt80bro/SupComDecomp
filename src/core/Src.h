#include <vector>
#include <string>
#include "LuaPlus.h"
#include "gpgcore/String.h"
#include "gpgcore/reflection/RRef.h"
#include "gpgcore/streams/Stream.h"

namespace Moho {

using color_t = unsigned int;

Moho::color_t SCR_DecodeColor(LuaPlus::LuaState *, const LuaPlus::LuaObject &); // 0x004B2D20
Moho::color_t SCR_DecodeColor(const std::string &); // 0x004B2D80
LuaPlus::LuaObject SCR_EncodeColor(LuaPlus::LuaState *, Moho::color_t); // 0x004B2E60

bool SCR_LuaDoScript(LuaPlus::LuaState *L, gpg::StrArg file, LuaPlus::LuaObject *out); // 0x004CEA20
void SCR_LuaDoFileConcat(LuaPlus::LuaState *L, std::vector<std::string> files, LuaPlus::LuaObject *out); // 0x004CECD0
void SCR_RObjectToLuaMerge(const gpg::RRef &, LuaPlus::LuaObject &); // 0x004CF0B0
LuaPlus::LuaObject SCR_RObjectToLua(const gpg::RRef &, LuaPlus::LuaState *); // 0x004CF4A0
bool SCR_LuaBuildObject(LuaPlus::LuaObject, const gpg::RRef &, bool); // 0x004CF510

void SCR_ExpectArgs(LuaPlus::LuaState *L, int amt, char const *msg); // 0x004D2350
void SCR_ExpectArgs(LuaPlus::LuaState *L, int min, int max, char const *msg); // 0x004D2380
std::string SCR_QuoteLuaString(gpg::StrArg); // 0x004D23D0
void SCR_GetEnum(LuaPlus::LuaState *, gpg::StrArg, gpg::RRef &); // 0x004D2550
LuaPlus::LuaObject SCR_Copy(const LuaPlus::LuaObject &, LuaPlus::LuaState *); // 0x004D26D0
LuaPlus::LuaObject SCR_FromByteStream(gpg::Stream &, LuaPlus::LuaState *); // 0x004D2A40
bool SCR_ToByteStream(const LuaPlus::LuaObject &, gpg::Stream &); // 0x004D2C80
std::string SCR_ToString(const LuaPlus::LuaObject &); // 0x004D2F70
LuaPlus::LuaObject SCR_FromString(const std::string &, LuaPlus::LuaState *); // 0x004D3110
bool SCR_ToBool(const LuaPlus::LuaObject &); // 0x004D31D0
LuaPlus::LuaObject SCR_Import(LuaPlus::LuaState *, gpg::StrArg); // 0x004D3250
bool SCR_ModuleExists(LuaPlus::LuaState *, gpg::StrArg); // 0x004D3370
std::string SCR_Traceback(LuaPlus::LuaState *, gpg::StrArg); // 0x004D33A0


}
