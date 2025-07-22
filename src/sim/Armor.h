#include <list>
#include <string>
#include "LuaPlus.h"
#include "gpgcore/String.h"

namespace Moho {

std::list<std::string> ARMOR_GetArmorDefinations/*sic*/(LuaPlus::LuaState *state, gpg::StrArg); // 0x00564660

}
