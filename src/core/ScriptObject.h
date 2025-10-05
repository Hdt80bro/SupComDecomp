#include "core/TDatList.h"
#include "gpgcore/reflection/reflection.h"
#include "gpgcore/containers/fastvector.h"
#include "LuaPlus.h"

namespace Moho {

// 0x00E0926C
class CScriptObject :
    public gpg::RObject,
    public Moho::TDatListItem<Moho::CScriptObject, void>
{
public:
    LuaPlus::LuaObject mCObj;
    LuaPlus::LuaObject mLuaObj;

    static gpg::RType *StaticGetClass(); // 0x004C7BE0
    ~CScriptObject() override; // 0x004C6FF0
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
    template<class... Ts>
    bool RunScriptBool(const char *name, Ts... args) {
        LuaPlus::LuaObject script = this->FindScript(name);
        if (! script.IsNil()) {
            LuaPlus::LuaFunction fn{script};
            return fn(LuaPlus::LuaObject{args}...);
        }
        return false;
    }
};

template<class T>
class CScriptLazyVar
{
public:
    LuaPlus::LuaObject mObj;

    CScriptLazyVar(LuaPlus::LuaState *state) {

    }

    operator LuaPlus::LuaObject&() {
        return this->mObj;
    }
    operator T() {

    }
    void operator=(T val) {

    }
};


/*
template<>
class CScriptLazyVar<float>
{
public:
    CSCriptLazyVar(LuaPlus::LuaState *); // 0x007836E0
    operator float(); // 0x00783840
    void operator=(float val); // 0x007839E0
}
*/

}
