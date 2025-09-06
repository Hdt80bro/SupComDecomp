#include "gpgcore/reflection/reflection.h"
#include "LuaPlus.h"
#include <string>

namespace Moho {

class RRuleGameRules;
class RResId;

// 0x00E0E0A8
class RBlueprint : public gpg::RObject
{
public:
    Moho::RRuleGameRules *mOwner;
    std::string mName;
    std::string mDesc;
    std::string mSource;
    unsigned int mBlueprintOrdinal;

    ~RBlueprint() override = default; // 0x0050DE40
    virtual void OnInitBlueprint() {} // 0x0050DBA0

    int GetBlueprintOrdinal() const; // MohoEngine.dll
    const char *GetBlueprintId() const; // MohoEngine.dll
    const char *GetDescription() const; // MohoEngine.dll
    const char *GetSource() const; // MohoEngine.dll
    RBlueprint(const Moho::RBlueprint &); // MohoEngine.dll
    RBlueprint(const Moho::RRuleGameRules *, const Moho::RResId &); // 0x0050DD60
    void InitBlueprint(LuaPlus::LuaObject &); // 0x0050DF10
    LuaPlus::LuaObject GetLuaBlueprint(LuaPlus::LuaState *) const; // 0x0050DF90
};

std::string BP_ShortId(const std::string &); // 0x0050E010

}
