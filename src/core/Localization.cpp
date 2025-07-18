#include "Localization.h"

std::string Moho::Loc(LuaPlus::LuaState *state, const char *key) {
    LuaPlus::LuaFunction fn{state->GetGlobal("LOC")};
    return std::string{fn(key)};
}
