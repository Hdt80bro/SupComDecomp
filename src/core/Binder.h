#include "gpgcore/reflection/reflection.h"
#include "gpgcore/streams/Stream.h"
#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"
#include "LuaPlus.h"
#include <string>
#include <vector>


namespace Moho {

// 0x00E01720
class CScrLuaObjectFactory
{
public:
    int mIndex;

    virtual LuaPlus::LuaObject Create(LuaPlus::LuaState *) = 0;

    LuaPlus::LuaObject Get(LuaPlus::LuaState *); // 0x004CCE70
};

template<class T>
class CScrLuaMetatableFactory : public Moho::CScrLuaObjectFactory
{
public:
    static Moho::CScrLuaMetatableFactory<T> sInstance;

    LuaPlus::LuaObject Create(LuaPlus::LuaState *L) {
        return Moho::SCR_CreateSimpleMetatable(L);
    }
};

// 0x00E00E8C
class CScrLuaBinder
{
public:
    const char *mMethodName;
    const char *mClassName;
    const char *mHelp;
    Moho::CScrLuaBinder *mPrevDef;
    lua_CFunction mFunc;
    Moho::CScrLuaObjectFactory *mFactory;

    virtual void Run(LuaPlus::LuaState *); // 0x004CD3A0
};




// 0x00E0A6BC
class CScrLuaInitForm : public Moho::CScrLuaBinder
{
public:
    void Run(LuaPlus::LuaState *) override; // 0x004CD3A0

    CScrLuaInitForm(Moho::CScrLuaInitFormSet &, const char *, const char *, const char *); // 0x004CD370
};

#define MOHO_LUA_FUNC(set, func, help) MOHO_LUA_BIND(set, "<global>", func, help, func)
#define MOHO_LUA_METHOD(set, clazz, method, help) MOHO_LUA_BIND(set, #clazz, method, help, clazz##method)
#define MOHO_LUA_BIND(set, className, func, help, binder) MOHO_LUA_BIND_ADAPTER(set, className, func, help, binder##_LuaFuncDef, binder##_LuaFunc)
#define MOHO_LUA_BIND_ADAPTER(set, className, func, help, binder, adapterFunc)\
int func(LuaPlus::LuaState *);\
int adapterFunc(lua_State *L) {\
    func((LuaPlus::LuaState *) L->stateUserData);\
}\
class binder : public Moho::CScrLuaInitForm\
{\
public:\
    binder() : Moho::CScrLuaInitForm {\
        set, #func, className, help\
    } {\
        this->mFunc = &adapterFunc;\
    }\
};\
static binder sInstance{};\
int func(LuaPlus::LuaState *state)




// 0x00E1D7B8
class CScrLuaBaseClassSpec : Moho::CScrLuaInitForm
{
public:
    void Run(LuaPlus::LuaState *) override; // 0x004CD5E0

    CScrLuaBaseClassSpec(Moho::CScrLuaInitFormSet &, Moho::CScrLuaObjectFactory *, Moho::CScrLuaObjectFactory *, const char *, const char *); // 0x005BD5E0
};




// 0x00E072A8
class CScrLuaClassBinder : public Moho::CScrLuaInitForm
{
public:
    void Run(LuaPlus::LuaState *) override; // 0x004CD460
};

struct CScrLuaInitFormSet
{
    static Moho::CScrLuaInitFormSet *sSets;

    const char *mName;
    Moho::CScrLuaInitForm *mForms;
    bool mRegistered;
    Moho::CScrLuaInitFormSet *mNextSet;

    CScrLuaInitFormSet(const char *name); // 0x004CCF80
    void AddInit(Moho::CScrLuaInitForm *); // 0x004CCFA0
    void RunInits(LuaPlus::LuaState *); // 0x004CCFB0
    void DumpDocs(); // 0x004CCFE0
};




#define MOHO_LUA_EXPECT_ARGS(count) do {\
    int numargs = state->GetTop();\
    if (numargs != count) {\
        state->Error("%s\n  expected %d args, but got %d", sInstance.mHelp, count, numargs);\
    }\
} while (false)

#define MOHO_LUA_EXPECT_ARGS_RANGE(min, max) do {\
    int numargs = state->GetTop();\
    if (numargs < min || max < numargs) {\
        state->Error("%s\n  expected between %d and %d args, but got %d", sInstance.mHelp, min, max, numargs);\
    }\
} while (false)


}
