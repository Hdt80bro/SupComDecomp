#include "core/TDatListItem.h"
#include "gpgcore/reflection/reflection.h"
#include "gpgcore/containers/fastvector.h"
#include "LuaPlus.h"

namespace Moho {

// 0x00E0926C
class CScriptObject :
    public Moho::TDatListItem<Moho::CScriptObject>
{
public:
    LuaPlus::LuaObject mCObj;
    LuaPlus::LuaObject mLuaObj;

    static gpg::RType *StaticGetClass(); // 0x004C7BE0
    virtual gpg::RType *GetClass() const = 0;
    virtual gpg::RRef GetDerivedObjectRef() = 0;
    virtual ~CScriptObject(); // 0x004C6FF0
    virtual std::string GetErrorDescription(); // 0x004C70A0

    void CreateLuaObject(const LuaPlus::LuaObject &, const LuaPlus::LuaObject &, const LuaPlus::LuaObject &, const LuaPlus::LuaObject &); // 0x004C70D0
    void SetLuaObject(const LuaPlus::LuaObject &); // 0x004C72D0
    void LogScriptWarning(Moho::CScriptObject *, const char *, const char *); // 0x004C7410
    LuaPlus::LuaObject FindScript(const char *); // 0x004C74B0
    bool RunScriptMultiRet(const char *, gpg::fastvector<LuaPlus::LuaObject> &, LuaPlus::LuaObject, LuaPlus::LuaObject, LuaPlus::LuaObject, LuaPlus::LuaObject, LuaPlus::LuaObject); // 0x004C7580

    template<class... Ts>
    LuaPlus::LuaObject RunScript(const char *name, Ts... args) {
        LuaPlus::LuaObject script = this->FindScript(name);
        if (! script.IsNil()) {
            LuaPlus::LuaFunction fn{script};
            return fn(LuaPlus::LuaObject{args}...);
        }
    }
};

}
