#include "gpgcore/reflection/RRef.h"
#include "gpgcore/streams/Stream.h"
#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"
//#include "wx/frame.h"
#include "LuaPlus.h"
#include <string>
#include <vector>

namespace Moho {

// 0x00E0863C
class ScrDebugWindow : public wxFrame
{
    // ...
};

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
    Moho::CScrLuaBinder *mPreviousDef;
    lua_CFunction mGunc;
    Moho::CScrLuaObjectFactory *mFactory;

    virtual void Run(LuaPlus::LuaState *); // 0x004CD3A0
};

// 0x00E0A6BC
class CScrLuaInitForm : public Moho::CScrLuaBinder
{
};

// 0x00E072A8
class CScrLuaClassBinder : public Moho::CScrLuaInitForm
{
};

struct CScrLuaInitFormSet
{
    const char *mName;
    Moho::CScrLuaInitForm *mForms;
    bool mRegistered;
    Moho::CScrLuaInitFormSet *mNextSet;
};

// 0x00E076B8
class ScrBreakpoint
{
public:
    int v1;
    std::string mName;
    int v2;

    virtual ~ScrBreakpoint() = default; // 0x004B0130
};

// 0x00E0767C
class ScrActivation
{
public:
    std::string mFile;
    std::string mName;
    int mLine;

    virtual ~ScrActivation() = default; // 0x004AFF60
};

// 0x00E07E58
class ScrWatch
{
    std::string mFile;
    LuaPlus::LuaObject mObj;

    virtual ~ScrWatch() = default; // 0x004B9070
};




using color_t = unsigned int;

Moho::color_t SCR_DecodeColor(LuaPlus::LuaState *, const LuaPlus::LuaObject &); // 0x004B2D20
Moho::color_t SCR_DecodeColor(const std::string &); // 0x004B2D80
LuaPlus::LuaObject SCR_EncodeColor(LuaPlus::LuaState *, Moho::color_t); // 0x004B2E60
void SCR_CreateDebugWindow(); // 0x004B4800
void SCR_DestroyDebugWindow(); // 0x004B4890
void SCR_HookState(LuaPlus::LuaState *); // 0x004B49B0
void SCR_UnhookState(LuaPlus::LuaState *); // 0x004B49D0
void SCR_AddBreakpoint(const Moho::ScrBreakpoint &); // 0x004B4A30
void SCR_EnableBreakpoint(const Moho::ScrBreakpoint &, bool); // 0x004B4AC0
void SCR_EnableAllBreakpoints(bool); // 0x004B4B50
void SCR_RemoveBreakpoint(const Moho::ScrBreakpoint &); // 0x004B4BF0
void SCR_RemoveAllBreakpoints(); // 0x004B4C80
void SCR_EnumerateBreakpoints(const std::string &, std::vector<Moho::ScrBreakpoint> &); // 0x004B4D20
void SCR_EnumerateCallStack(std::vector<Moho::ScrActivation> &); // 0x004B4DF0
void SCR_EnumerateLocals(int, std::vector<Moho::ScrWatch> &); // 0x004B5140
void SCR_EnumerateGlobals(std::vector<Moho::ScrWatch> &); // 0x004B5470
void SCR_DebugStep(); // 0x004B5690
void SCR_DebugResume(); // 0x004B5790
void SCR_LoadBreakpoints(); // 0x004B5840
void SCR_SaveBreakpoints(); // 0x004B59B0
void SCR_ConcatArgsAndCall(LuaPlus::LuaState *, char, void (*)(LuaPlus::LuaState *, const char *)); // 0x004CD740
bool SCR_LuaDoString(LuaPlus::LuaState *, gpg::StrArg); // 0x004CDBA0
void SCR_AddHookDirectory(gpg::StrArg); // 0x004CDF60
bool SCR_LuaDoFile(LuaPlus::LuaState *, gpg::StrArg, LuaPlus::LuaObject *); // 0x004CE020
bool SCR_LuaDoScript(LuaPlus::LuaState *L, gpg::StrArg file, LuaPlus::LuaObject *out); // 0x004CEA20
void SCR_LuaDoFileConcat(LuaPlus::LuaState *L, std::vector<std::string> files, LuaPlus::LuaObject *out); // 0x004CECD0
void SCR_RObjectToLuaMerge(const gpg::RRef &, LuaPlus::LuaObject &); // 0x004CF0B0
LuaPlus::LuaObject SCR_RObjectToLua(const gpg::RRef &, LuaPlus::LuaState *); // 0x004CF4A0
bool SCR_LuaBuildObject(LuaPlus::LuaObject, const gpg::RRef &, bool); // 0x004CF510

LuaPlus::LuaObject SCR_CreateSimpleMetatable(LuaPlus::LuaState *); // 0x004D22D0
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

static int sCurThreadId; // 0x010A63A0
static Moho::ScrDebugWindow *sSrcDebugWindow; // 0x010A63A4
static std::vector<std::string> sHookDirs; // 0x010A91D0

struct struct_LuaFileLoaderDat
{
    gpg::MemBuffer<const char> mBuf;
    bool mDone;

    struct_LuaFileLoaderDat(gpg::MemBuffer<const char> &buf) :
        mBuf{buf},
        mDone{false}
    {} // inline e.g. 0x004CE0E2
};

bool func_ParseColor(const char *str, Moho::color_t *dest); // 0x004B2B90
void func_DebugLuaHook(lua_State *L, lua_Debug *ar); // 0x004B5C70
const lua_WChar *func_LuaFileLoader(lua_State *L, void *ud, size_t *sz); // 0x004CDCF0
