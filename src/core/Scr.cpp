#include "Scr.h"
#include "core/Disk.h"
#include "core/Exception.h"
#include "gpgcore/General.h"

int sCurThreadId; // 0x010A63A0
Moho::ScrDebugWindow *sSrcDebugWindow; // 0x010A63A4
std::vector<std::string> sHookDirs; // 0x010A91D0


// 0x004CCE70
LuaPlus::LuaObject Moho::CScrLuaObjectFactory::Get(LuaPlus::LuaState *state) {
    LuaPlus::LuaObject facObjs = state->GetGlobal("__factory_objects");
    if (facObjs.IsNil()) {
        facObjs.AssignNewTable(state, 0, 0);
        LuaPlus::LuaObject globals = state->GetGlobals();
        globals.SetObject("__factory_objects", &facObjs);
    }
    LuaPlus::LuaObject dest = facObjs.GetByIndex(this->mIndex);
    if (dest.IsNil()) {
        dest = this->Create(state);
        facObjs.SetObject(this->mIndex, dest);
    }
    return dest;
}

// 0x004CD3A0
void Moho::CScrLuaBinder::Run(LuaPlus::LuaState *state) {
    if (this->mFactory != nullptr) {
        this->mFactory->Get(state).Register(this->mMethodName, this->mFunc, 0);
    } else {
        state->GetGlobals().Register(this->mMethodName, this->mFunc, 0);
    }
}


// 0x004B2D20
Moho::color_t Moho::SCR_DecodeColor(LuaPlus::LuaState *state, const LuaPlus::LuaObject &obj) {
    if (! obj.IsString()) {
        state->Error("Invalid color, must be a string.");
        return 0;
    }
    Moho::color_t col = 0;
    if (! func_ParseColor(obj.GetString(), &col)) {
        state->Error("Unknown color: %s", obj.GetString());
        return 0;
    }
    return col;
}


// 0x004B2D80
Moho::color_t Moho::SCR_DecodeColor(const std::string &str) {
    Moho::color_t col = 0;
    if (! func_ParseColor(str.c_str(), &col)) {
        throw Moho::XDataError{gpg::STR_Printf("Unknown color: %s", str.c_str()).c_str()};
    }
    return col;
}


// 0x004B2E60
LuaPlus::LuaObject Moho::SCR_EncodeColor(LuaPlus::LuaState *state, Moho::color_t col) {
    LuaPlus::LuaObject obj{};
    char str[12];
    sprintf(str, "%08x", col);
    obj.AssignString(state, str);
    return obj;
}

// 0x004B4800
void Moho::SCR_CreateDebugWindow() {
    if (sSrcDebugWindow == nullptr) {
        sCurThreadId = GetCurrentThreadId();
        Moho::SCR_LoadBreakpoints();
        sSrcDebugWindow = new Moho::ScrDebugWindow{};
        sSrcDebugWindow->Show(true);
    }
}

// 0x0x004B4890
void Moho::SCR_DestroyDebugWindow() {
    if (sSrcDebugWindow != nullptr) {
        sSrcDebugWindow->Destroy();
        sSrcDebugWindow = nullptr;
        {
            boost::mutex::scoped_lock{preferences_lock};
            Moho::SCR_SaveBreakpoints();
            sub_4B7390((int *)global_breakpoint_itr.list->prev);
            global_breakpoint_itr.list->prev = global_breakpoint_itr.list;
            global_breakpoint_itr.k = 0;
            global_breakpoint_itr.list->next = global_breakpoint_itr.list;
            global_breakpoint_itr.list->str._Myfirstiter = (struct _Iterator_base *)global_breakpoint_itr.list;
            byte_10A6331 = 0;
        }
        {
            boost::mutex::scoped_lock{stru_10A8A50};
            sub_4B6F10(*(_DWORD *)(dword_10A8A48 + 4));
            *(_DWORD *)(dword_10A8A48 + 4) = dword_10A8A48;
            dword_10A8A4C = 0;
            *(_DWORD *)dword_10A8A48 = dword_10A8A48;
            *(_DWORD *)(dword_10A8A48 + 8) = dword_10A8A48;
            byte_10A6330 = 0;
        }
    }
}

// 0x004B49B0
void Moho::SCR_HookState(LuaPlus::LuaState *state) {
    if (sSrcDebugWindow != nullptr) {
        state->SetHook(func_DebugLuaHook, LUA_MASKLINE, 0);
    }
}

// 0x004B49D0
void Moho::SCR_UnhookState(LuaPlus::LuaState *state) {
    state->SetHook(nullptr, 0, 0);
}



// 0x004CDBA0
bool Moho::SCR_LuaDoString(LuaPlus::LuaState *state, gpg::StrArg str) {
    int top = state->GetTop();
    bool ret = true;
    if (state->LoadBuffer(str, strlen(str), str) || state->Call(0, -1)) {
        LuaPlus::LuaStackObject obj{state, -1};
        gpg::Warnf("Error running lua command: %s", obj.GetString());
        ret = false;
    }
    state->SetTop(top);
    return ret;
}

// 0x004CDF60
void Moho::SCR_AddHookDirectory(gpg::StrArg dir) {
    sHookDirs.push_back(std::string{dir});
}

// 0x004CE020
bool SCR_LuaDoFile(LuaPlus::LuaState *state, gpg::StrArg filename, LuaPlus::LuaObject *env) {
    int top = state->GetTop();
    {
        gpg::MemBuffer<const char> memmap = Moho::DISK_MemoryMapFile(filename);
        if (memmap.begin() == nullptr) {
            gpg::Warnf("Can't open lua file \"%s\"", filename);
            state->SetTop(top);
            return false;
        }
        struct_LuaFileLoaderDat dat{memmap};
        if (lua_load(state->m_state, func_LuaFileLoader, &dat, gpg::STR_Printf("@%s", filename).c_str())) {
            LuaPlus::LuaStackObject obj{state, -1}
            gpg::Warnf("Loading \"%s\" failed: %s", filename, obj.GetString());
            state->SetTop(top);
            return false;
        }
    }
    if (env != nullptr) {
        env->PushStack(state);
        state->SetGlobals(-2);
    }
    if (state->Call(0, 0)) {
        LuaPlus::LuaStackObject obj{state, -1};
        gpg::Warnf("Running \"%s\" failed: %s", filename, obj.GetString());
        state->SetTop(top);
        return false;
    }
    state->SetTop(top);
    return true;
}



// 0x004CDCF0
const lua_WChar *func_LuaFileLoader(lua_State *L, void *ud, size_t *sz) {
    auto dat = (struct_LuaFileLoaderDat *) ud;
    if (dat->mBuff.begin() == nullptr) {
        return nullptr;
    }
    int size = dat->mBuff.Size();
    if (size == 0 || dat->mDone) {
        return nullptr;
    }
    *sz = size;
    dat->mDone = true;
    return (const lua_WChar *) dat->mBuff.begin();
}
