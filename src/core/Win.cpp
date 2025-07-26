#include "Win.h"


WSupComFrame *mainWindow; // 0x010A63B8
CScApp *supcomapp; // 0x010A63BC
HHOOK windowHook; // 0x010A63C0
gpg::time::Timer wakeupTimer; // 0x010A9B78
float wakeupTimerDur; // 0x010A9B84


// 0x004F1190
void Moho::WIN_ShowCrashDialog(const char *arg0, const char *a4, _EXCEPTION_POINTERS *a2, int ecx0) {
    std::ostringstream strm{2, 1};
    strm << a4 << "\n\n";
    WCHAR Filename[512];
    if (GetModuleFileNameW(0, Filename, sizeof(Filename)) != nullptr) {
        strm << "Program : " << = gpg::STR_WideToUtf8(Filename) << "\n";
    } else {
        strm << "Program : <unknown>\n";
    }
    strm << "Cmd line arguments : " << Moho::CFG_GetArgs() << "\n" << "\n";
    strm << "Callstack:\n";

    void *stack = nullptr;
    if (a2) {
        stack = (void *) a2->ContextRecord;
    } else {
        ecx0 += 2;
    }
    unsigned int a3[64];
    int v12 = Moho::PLAT_GetCallStack(stack, sizeof(a3), a3);
    if (v12 <= ecx0) {
        strm << "    unavailable.\n";
    } else {
        strm << Moho::PLAT_FormatCallstack(ecx0, v12, a3);
    }
    strm << "\n" << "Last 100 lines of log...\n\n";
    strm << Moho::LOG_GetRecentLines();
    std::string v23 = strm.str();
    HMODULE v21;
    GetModuleHandleExW(6u, (LPCWSTR)DialogFunc, &v21);
    HMODULE phModule[3];
    phModule[1] = v23.c_str();
    phModule[0] = arg0;
    if (DialogBoxParamW(v21, (LPCWSTR)0x7A, 0, DialogFunc, (LPARAM)phModule) == -1) {
        gpg::Logf("DialogBoxParam failed: %s", Moho::WIN_GetLastError().c_str());
    }
}

// 0x004F1500
void Moho::WIN_CrashDialogDieHandler(const char *msg) {
    while (true) {
        Moho::WIN_ShowCrashDialog(2, 0, (HMODULE)"Fatal Error", msg);
        __debugbreak();
    }
}

// 0x004F20B0
void Moho::WIN_AppExecute(Moho::IWinApp *app) {
    if (app == nullptr) {
        return;
    }
    supcomapp = this;
    windowHook = SetWindowsHookExW(
        WH_KEYBOARD_LL,
        func_WindowHook,
        GetModuleHandleW(nullptr),
        0
    );
    HMODULE phModule;
    GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT|GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
        (LPCWSTR)func_WindowHook,
        &phModule
    );
    wxEntry(phModule, 0, 0, 0, 0);
    if (func_CorrectPlatformVersion()) {
        Moho::THREAD_SetAffinity(1);
        InitCommonControls();
        CoInitialize(0);
        Moho::PLAT_Init();
        Moho::PLAT_CatchStructuredExceptions();
        wakeupTimer = gpg::time::Timer{};
        if (! app->Main()) {
            TerminateProcess(GetCurrentProcess(), 1u);
        }
        wxTheApp->m_exitOnFrameDelete = true;
        wxTheApp->m_keepGoing = true;
        _controlfp(0x20000, 0x30000);
        bool success = true;
        bool acceptNewEvent = true;
        while (true) {
            while (success) {
                while (true) {
                    while (acceptNewEvent) {
                        SleepEx(0, true);
                        func_UserFrame(Moho::WIN_GetBeforeEventsStage());
                        acceptNewEvent = false;
                    }
                    if (! wxTheApp->Pending()) {
                        break;
                    }

                    wxTheApp->Dispatch1();
                    success = true;
                }
                if (success) {
                    success = wxTheApp->ProcessIdle();
                }
            }
            if (! wxTheApp->m_keepGoing) {
                break;
            }

            app->OnNoMoreEvents();
            success = true;
            acceptNewEvent = true;
            func_UserFrame(Moho::WIN_GetBeforeWaitStage());
            float waitTime = wakeupTimerDur - func_ProbeWakeTimer();
            if (waitTime >= 0.0) {
                if (waitTime <= 4294967300.0) {
                    waitTime = (int) waitTime;
                } else {
                    waitTime = NAN;
                }
            } else {
                waitTime = 0.0;
            }
            wakeupTimer = pInf;
            Moho::WIN_GetWaitHandleSet().MsgWaitEx(waitTime);
        }
        app->Func1();
        Moho::WINX_Exit();
        Moho::PLAT_Exit();
        if (wxTheApp != nullptr) {
            bool cont = true;
            do {
                bool pend;
                do {
                    bool pend = false;
                    if (wxTheApp->Pending()) {
                        wxTheApp->Dispatch1();
                        pend = true;
                        cont = true;
                    }
                } while (pend);
            } while (cont && cont = wxTheApp->ProcessIdle());
            wxTheApp->OnExit();
            wxApp::CleanUp();
        }
        if (windowHook != NULL) {
            UnhookWindowsHookEx(windowHook);
        }
        Moho::RES_Exit();
    } else {
        std::string title{"Old OS Version"};
        std::string body{
            "This application requires Windows NT, 2000, XP, or newer, to operate.\n"
            "Windows 95, 98, and ME are not supported."
        };
        Moho::WIN_OkBox(title.c_str(), body.c_str());
    }
    supcomapp = nullptr;
}

// 0x004F2400, 0x004F2B40
void Moho::WIN_AppRequestExit(){
    wxTheApp->ExitMainLoop();
}

// 0x004F2410
Moho::IWinApp *Moho::WIN_GetCurrentApp() {
    return supcomapp;
}

// 0x004F2420
Moho::CWaitHandleSet *Moho::WIN_GetWaitHandleSet() {
    static Moho::CWaitHandleSet cwaithandleset{}; // 0x01103AE0
    return &cwaithandleset;
}

// 0x004F2480
Moho::CTaskStage *Moho::WIN_GetBeforeEventsStage() {
    static Moho::CTaskStage beforeEventsStage{}; // 0x011043CC
  return &beforeEventsStage;
}

// 0x004F24F0
Moho::CTaskStage *Moho::WIN_GetBeforeWaitStage() {
    static Moho::CTaskStage beforeWaitStage; // 0x011043B4
    return &beforeWaitStage;
}

// 0x004F2560
void Moho::WIN_SetWakeupTimer(float time) {
    if (time < 0.0) {
        time = 0.0;
    } else {
        time += func_ProbeWakeTimer();
        if (time >= wakeupTimerDur) {
            return;
        }
    }
    wakeupTimerDur = time;
}

// 0x004F25B0
wxWindow *Moho::WIN_GetMainWindow() {
    return mainWindow;
}

// 0x004F25C0
void Moho::WIN_SetMainWindow(wxWindow *wind) {
    mainWindow = wind;
}

// 0x004F25D0
std::string Moho::WIN_GetClipboardText() {
    std::string ret;
    HWND handle;
    if (mainWindow != nullptr) {
        handle = mainWindow->GetHandle();
    } else {
        handle = 0;
    }
    if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(handle)) {
        HANDLE ClipboardData = GetClipboardData(CF_UNICODETEXT);
        if (ClipboardData) {
            auto dat = (const wchar_t *) GlobalLock(ClipboardData);
            if (dat != nullptr) {
                ret = gpg::STR_WideToUtf8(dat);
                GlobalUnlock(ClipboardData);
            }
        }
        CloseClipboard();
    }
    return ret;
}

// 0x004F2730
bool Moho::WIN_CopyToClipboard(const wchar_t *str) {
    size_t len = wcslen(str);
    HGLOBAL globStr = GlobalAlloc(0x2042u, 2 * (len + 1));
    bool succ = false;
    if (globStr != nullptr) {
        wchar_t *locked = (wchar_t *)GlobalLock(globStr);
        memcpy(locked, str, 2 * len);
        locked[len] = '\0';
        GlobalUnlock(globStr);
        HWND hndl = 0;
        if (mainWindow != nullptr) {
            hndl = mainWindow->GetHandle();
        }
        if (OpenClipboard(hndl)) {
            if (EmptyClipboard()) {
                succ = SetClipboardData(CF_UNICODETEXT, globStr);
            }
            CloseClipboard();
        }
    }
    if (! succ) {
        GlobalFree(globStr);
    }
    return succ;
}

// 0x004F2800
void __cdecl Moho::WIN_OkBox(const char *caption, const char *text) {
    HWND handle;
    if (mainWindow != nullptr) {
        handle = mainWindow->GetHandle();
    } else {
        handle = 0;
    }
    MessageBoxW(
        handle,
        gpg::STR_Utf8ToWide(text).ctr(),
        gpg::STR_Utf8ToWide(caption),
        MB_YESNO
    );
}

// 0x004F2900
bool Moho::WIN_YesNoBox(const char *caption, const char *text) {
    HWND handle;
    if (mainWindow != nullptr) {
        handle = mainWindow->GetHandle();
    } else {
        handle = 0;
    }
    return MessageBoxW(
        handle,
        gpg::STR_Utf8ToWide(text).ctr(),
        gpg::STR_Utf8ToWide(caption),
        MB_TOPMOST|MB_YESNO
    ) == IDYES;
}

// 0x004F2A00
std::string Moho::WIN_GetLastError() {
    int LastError = GetLastError();
    LPWSTR Buffer;
    int res = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,
        0, LastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &Buffer, 0, 0
    );
    if (res <= 0) {
        return gpg::STR_Printf("Unknown error 0x%08x", LastError);
    } else {
        std::string ret = gpg::STR_WideToUtf8(Buffer);
        LocalFree(Buffer);
        return ret;
    }
}

// 0x004F3A60
void Moho::WINX_Exit() {

}

// 0x004F3B60
wxString Moho::WINX_Printf(const char *args...) {

}

// 0x004F3CD0
void Moho::WINX_PrecreateLogWindow() {

}

// 0x004F3CE0
void Moho::WINX_InitSplash(gpg::StrArg) {

}

// 0x004F3F30
void Moho::WINX_ExitSplash() {

}

// 0x004F67E0
void Moho::WINX_PrecreateLogWindow() {

}

// 0x0041B560
bool Moho::CFG_GetArgOption(const char *flag, unsigned int a1, std::vector<std::string> *store) {
    if (flag == nullptr) {
        return false;
    }
    if (_argc - args <= 1) {
        return false;
    }
    int pos = 1;
    while (stricmp(_argv[pos], flag) != 0) {
        if (++pos >= _argc - args) {
            return false;
        }
    }
    if (store != nullptr && args != 0) {
        do {
            store->push_back(std::string{_argv[++pos]});
            --args;
        } while (args);
    }
    return true;
}

// 0x0041B690
std::string Moho::CFG_GetArgs() {
    std::string builder{""};
    for (int i = 1; i < _argc; ++i) {
        builder.append{gpg::STR_Printf("%s ", _argv[i]);
    }
    return gpg::STR_Chop(builder.c_str(), '\0');
}

// 0x004091F0
int func_Dispatch(Moho::CTaskThread *thrd) {
    if (0 < --thrd->val1) {
        return 0;
    }
    while (true) {
        Moho::CTask *task = thrd->task;
        if (task == nullptr) {
            return -1;
        }
        bool v10 = false;
        task->destroyed = &v10;
        int res = task->Dispatch();
        if (! v10) {
            task->destroyed = nullptr;
        }
        switch (res) {
            case -4:
                return -2;
            case -3:
                thrd->Destroy();
                return -1;
            case -2:
                thrd->Stage();
                return 0;
            case -1: {
                Moho::CTask **i;
                for (i = &thrd->task; *i != task; i = &(*i)->subtask)
                    {}
                *i = task->subtask;
                task->subtask = nullptr;
                task->taskThread = nullptr;
                if (task->isOwning) {
                    delete(task);
                }
                break;
            }
            case 0: {
                thrd->val1 = 0;
                if (thrd->staged) {
                    return 0;
                }
                break;
            }
            case 1:
                thrd->val1 = 1;
                return 0;
            default:
                thrd->val1 = res - 1;
                return 0;
        }
    }
}

void func_UserFrame(Moho::CTaskStage *stage) {
    Moho::TDatListItem_CTaskThread *v2; // ecx
    Moho::TDatListItem_CTaskThread *next; // esi
    int v4; // eax
    Moho::TDatListItem_CTaskThread *v5; // edi
    Moho::TDatListItem_CTaskThread *prev; // eax
    Moho::TDatListItem_CTaskThread v7; // [esp+8h] [ebp-18h] BYREF
    int v8; // [esp+1Ch] [ebp-4h]

    v2 = &v7;
    Moho::TDatListItem<Moho::CTaskThread> v7;
    next = stage->threads.next;
    if (next != &stage->threads) {
        do {
            next->prev->next = next->next;
            next->next->prev = next->prev;
            next->prev = next;
            next->next = next;
            next->prev = v7.prev;
            next->next = &v7;
            v7.prev = next;
            next->prev->next = next;
            v4 = func_Dispatch((Moho::CTaskThread *)next);
            if (v4 == -2) {
                next->prev->next = next->next;
                next->next->prev = next->prev;
                next->prev = next;
                next->next = next;
                next->prev = stage->threads.prev;
                next->next = &stage->threads;
                stage->threads.prev = next;
                next->prev->next = next;
            } else if (v4 == -1) {
                delete(next)
            }
            next = stage->threads.next;
        }
        while ( next != (Moho::TDatListItem_CTaskThread *)stage );
        v2 = v7.next;
    }
    v5 = stage->threads.next;
    if (v2 == &v7) {
        v7.prev->next = v2;
        v7.next->prev = v7.prev;
    } else {
        prev = v5->prev;
        prev->next = v2;
        v5->prev = v7.prev;
        v7.prev->next = v5;
        v7.next->prev = prev;
        v7.prev = &v7;
        v7.next = &v7;
    }
}

// 0x004F1540
float func_ProbeWakeTimer() {
    return gpg::time::CyclesToMilliseconds(
        wakeupTimer.ElapsedCyclesAndReset()
    );
}

// 0x004F2000
bool func_CorrectPlatformVersion() {
    _OSVERSIONINFOW VersionInformation;
    ZeroMemory(&VersionInformation, sizeof(VersionInformation));
    VersionInformation.dwOSVersionInfoSize = sizeof(VersionInformation);
    if (GetVersionExW(&VersionInformation)) {
        return VersionInformation.dwPlatformId != 1;
    } else {
        return true;
    }
}

// 0x004F2050
LRESULT func_WindowHook(int code, WPARAM wParam, _DWORD *lParam) {
    if (code == 0
        && supcomapp->HasFrame()
        && (wParam == 0x100 || wParam == 0x101)
        && (*lParam == '[' || *lParam == '\\')
    ) {
        return 1;
    } else {
        return CallNextHookEx(windowHook, code, wParam, (LPARAM)lParam);
    }
}
