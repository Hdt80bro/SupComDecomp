#include "LuaPlus.h"
#include "core/TDatListItem.h"

namespace Moho {

// 0x00E0926C
class CScriptObject :
    public Moho::TDatListItem<Moho::CScriptObject>
{
public:
    static gpg::RType *StaticGetClass(void); // 0x

public:
    LuaPlus::LuaObject mCObj;
    LuaPlus::LuaObject mLuaObj;

    virtual gpg::RType *GetClassType() const = 0;
    virtual gpg::RRef GetDerivedObjectRef() = 0;
    virtual ~CScriptObject(); // 0x004C6FF0
    virtual std::string GetErrorDescription(); // 0x004C70A0

    void CreateLuaObject(const LuaPlus::LuaObject &, const LuaPlus::LuaObject &, const LuaPlus::LuaObject &, const LuaPlus::LuaObject &); // 0x004C70D0
    void SetLuaObject(const LuaPlus::LuaObject &); // 0x004C72D0
    void LogScriptWarning(Moho::CScriptObject *, const char *, const char *); // 0x004C7410
    LuaPlus::LuaObject FindScript(const char *); // 0x004C74B0
    bool RunScriptMultiRet(const char *, gpg::fastvector<LuaPlus::LuaObject> &, LuaPlus::LuaObject, LuaPlus::LuaObject, LuaPlus::LuaObject, LuaPlus::LuaObject, LuaPlus::LuaObject); // 0x004C7580
};

}
