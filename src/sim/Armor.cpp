#include "Armor.h"
#include "Src.h"
#include "gpgcore/General.h"

std::list<std::string> Moho::ARMOR_GetArmorDefinations(LuaPlus::LuaState *L, gpg::StrArg arm) {
    std::list<std::string> armDefs{};
    LuaPlus::LuaObject defModule = Moho::SCR_Import(L, "/lua/armordefinition.lua");
    if (defModule.IsNil()) {
        gpg::Warnf("can't load the armordefinition module -- no armor for you.");
    } else {
        LuaPlus::LuaObject defs = defModule.GetByName("armordefinition");
        if (defs.IsTable()) {
            for (int k = 1; k <= defs.GetCount(); ++k) {
                LuaPlus::LuaObject def = defs[k];
                if (stricmp(def[1].GetString(), arm) == 0) {
                    for (int i = 2; i <= def.GetCount(); ++i) {
                        armDefs.push_back(std::string{def[i].GetString()});
                    }
                    break;
                }
            }
        } else {
            gpg::Warnf("The armor module didn't define any armors.  Hmm Odd?");
        }
    }
    return armDefs;
}
