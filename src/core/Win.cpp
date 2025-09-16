#include "Win.h"
#include <sstream>


wxFrame *mainWindow; // 0x010A63B8
Moho::IWinApp *supcomapp; // 0x010A63BC
HHOOK windowHook; // 0x010A63C0
gpg::time::Timer wakeupTimer; // 0x010A9B78
float wakeupTimerDur; // 0x010A9B84
wxSplashScreen *splash_screen_ptr; // 0x010A9BE4
wxPNGHandler *png_Handler; // 0x010C6D48


inline HANDLE func_GetMainWindowHandle() {
    if (Moho::WIN_GetMainWindow() != nullptr) {
        return Moho::WIN_GetMainWindow()->GetHandle();
    } else {
        return 0;
    }
}

// 0x004F1190
void Moho::WIN_ShowCrashDialog(const char *arg0, const char *a4, _EXCEPTION_POINTERS *a2, int ecx0) {
    std::stringstream strm{};
    strm << a4 << "\n\n";
    WCHAR filename[512];
    if (GetModuleFileNameW(0, filename, sizeof(filename)) != nullptr) {
        strm << "Program : " << gpg::STR_WideToUtf8(filename) << "\n";
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
        Moho::WIN_ShowCrashDialog("Fatal Error", msg, nullptr, 2);
        __debugbreak();
    }
}

// 0x004F20B0
void Moho::WIN_AppExecute(Moho::IWinApp *app) {
    if (app == nullptr) {
        return;
    }
    sSupComApp = app;
    sWindowHook = SetWindowsHookExW(
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
    if (func_HasCorrectPlatform()) {
        Moho::THREAD_SetAffinity(1);
        InitCommonControls();
        CoInitialize(0);
        Moho::PLAT_Init();
        Moho::PLAT_CatchStructuredExceptions();
        sWakeupTimer = gpg::time::Timer{};
        if (! app->AppInit()) {
            TerminateProcess(GetCurrentProcess(), 1);
        }
        _controlfp(_PC_24, _MCW_PC);

        //wxTheApp->OnRun();
        wxTheApp->SetExitOnFrameDelete(true);

        //wxTheApp->MainLoop();
        wxTheApp->m_keepGoing = true;
        while (true) {
            SleepEx(0, true);
            func_UserFrame(Moho::WIN_GetBeforeEventsStage());
            bool success = true;
            while (success) {
                if (wxTheApp->Pending()) {
                    wxTheApp->Dispatch();
                    success = true;
                } else if (success) {
                    success = wxTheApp->ProcessIdle();
                }
            }
            if (! wxTheApp->m_keepGoing) {
                break;
            }
            // wxTheApp->DoMessage();
            app->Main();
            func_UserFrame(Moho::WIN_GetBeforeWaitStage());
            float waitTime = wakeupTimerDur - func_ProbeWakeTimer();
            if (waitTime < 0.0) {
                waitTime = 0.0;
            } else if (waitTime > 4294967300.0) {
                waitTime = NAN;
            } else {
                waitTime = (int) waitTime;
            }
            sWakeupTimer = INFINITY;
            Moho::WIN_GetWaitHandleSet()->MsgWaitEx(waitTime);
            //~DoMessage
        }
        //~MainLoop
        //~OnRun

        app->Destroy();
        Moho::WINX_Exit();
        Moho::PLAT_Exit();
        if (wxTheApp != nullptr) {
            wxTheApp->MainLoop();
            wxApp::CleanUp();
        }
        if (sWindowHook != NULL) {
            UnhookWindowsHookEx(sWindowHook);
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
    sSupComApp = nullptr;
}

// 0x004F2400, 0x004F2B40
void Moho::WIN_AppRequestExit(){
    wxTheApp->ExitMainLoop();
}

// 0x004F2410
Moho::IWinApp *Moho::WIN_GetCurrentApp() {
    return sSupComApp;
}

// 0x004F2420
Moho::CWaitHandleSet *Moho::WIN_GetWaitHandleSet() {
    static Moho::CWaitHandleSet sCWaitHandleSet{}; // 0x01103AE0
    return &sCWaitHandleSet;
}

// 0x004F2480
Moho::CTaskStage *Moho::WIN_GetBeforeEventsStage() {
    static Moho::CTaskStage sBeforeEventsStage{}; // 0x011043CC
    return &sBeforeEventsStage;
}

// 0x004F24F0
Moho::CTaskStage *Moho::WIN_GetBeforeWaitStage() {
    static Moho::CTaskStage sBeforeWaitStage; // 0x011043B4
    return &sBeforeWaitStage;
}

// 0x004F2560
void Moho::WIN_SetWakeupTimer(float time) {
    if (time < 0.0) {
        time = 0.0;
    } else {
        time += func_ProbeWakeTimer();
        if (time >= sWakeupTimerDur) {
            return;
        }
    }
    sWakeupTimerDur = time;
}

// 0x004F25B0
wxWindow *Moho::WIN_GetMainWindow() {
    return sMainWindow;
}

// 0x004F25C0
void Moho::WIN_SetMainWindow(wxWindow *wind) {
    sMainWindow = wind;
}

// 0x004F25D0
std::string Moho::WIN_GetClipboardText() {
    std::string ret;
    HWND handle = func_GetMainWindowHandle();
    if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(handle)) {
        HANDLE clipboardData = GetClipboardData(CF_UNICODETEXT);
        if (clipboardData) {
            auto dat = (const wchar_t *) GlobalLock(clipboardData);
            if (dat != nullptr) {
                ret = gpg::STR_WideToUtf8(dat);
                GlobalUnlock(clipboardData);
            }
        }
        CloseClipboard();
    }
    return ret;
}

// 0x004F2730
bool Moho::WIN_CopyToClipboard(const wchar_t *str) {
    size_t len = wcslen(str);
    HGLOBAL globStr = GlobalAlloc(GMEM_DDESHARE|GHND, 2 * (len + 1));
    bool succ = false;
    if (globStr != nullptr) {
        wchar_t *locked = (wchar_t *)GlobalLock(globStr);
        memcpy(locked, str, 2 * len);
        locked[len] = '\0';
        GlobalUnlock(globStr);
        if (OpenClipboard(func_GetMainWindowHandle())) {
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
void Moho::WIN_OkBox(const char *caption, const char *text) {
    HWND handle = func_GetMainWindowHandle();
    std::string wideText = gpg::STR_Utf8ToWide(text);
    std::string wideCaption = gpg::STR_Utf8ToWide(caption);
    MessageBoxW(handle, wideText.c_str(), wideCaption.c_str(), MB_YESNO);
}

// 0x004F2900
bool Moho::WIN_YesNoBox(const char *caption, const char *text) {
    HWND handle = func_GetMainWindowHandle();
    std::string wideText = gpg::STR_Utf8ToWide(text);
    std::string wideCaption = gpg::STR_Utf8ToWide(caption);
    return MessageBoxW(handle, wideText.c_str(), wideCaption.c_str(), MB_TOPMOST|MB_YESNO) == IDYES;
}

// 0x004F2A00
std::string Moho::WIN_GetLastError() {
    int lastError = GetLastError();
    LPWSTR buffer;
    int res = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,
        0, lastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &buffer, 0, 0
    );
    if (res <= 0) {
        return gpg::STR_Printf("Unknown error 0x%08x", lastError);
    } else {
        std::string ret = gpg::STR_WideToUtf8(buffer);
        LocalFree(buffer);
        return ret;
    }
}

// 0x004F3A60
void Moho::WINX_Exit() {
    // clean up managed frames and windows
}

// 0x004F3B60
wxString Moho::WINX_Printf(const char *fmt...) {
    va_list args;
    va_start(args, fmt);
    std::string str = gpg::STR_Utf8ToWide(gpg::STR_Va(fmt, args));
    return wxString{str.c_str()};
}

// 0x004F3CE0
void Moho::WINX_InitSplash(gpg::StrArg filename) {
    if (sPngHandler == nullptr) {
        sPngHandler = new wxPNGHandler{};
        wxImage::AddHandler(sPngHandler);
    }
    Moho::WINX_ExitSplash();
    wxBitmap bm{};
    if (bm.LoadFile(wxString{filename})) {
        wxSize size{1024, 768};
        tagRECT rect;
        if (GetWindowRect(0, &rect)) {
            size.x = min(1600, rect.right - rect.left);
            size.y = min(1200, rect.top - rect.bottom);
        }
        auto splash = new wxSplashScreen{
            wxBitmap{bm.ConvertToImage().Rescale(size.x, size.y)},
            2, nullptr, nullptr, -1,
            &wxDefaultPosition, &size,
            wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR
        };
        if (sSplashScreenPtr != nullptr) {
            delete(sSplashScreenPtr);
        }
        sSplashScreenPtr = splash;
    }
}

// 0x004F3F30
void Moho::WINX_ExitSplash() {
    if (sSplashScreenPtr != nullptr) {
        delete(sSplashScreenPtr);
        sSplashScreenPtr = nullptr;
    }
}
// 0x004F3CD0 -> 0x004F67E0
void Moho::WINX_PrecreateLogWindow() {

}

// 0x0041B560
bool Moho::CFG_GetArgOption(const char *opt, unsigned int args, std::vector<std::string> *store) {
    if (opt == nullptr) {
        return false;
    }
    if (__argc - args <= 1) {
        return false;
    }
    int pos = 1;
    while (stricmp(__argv[pos], opt) != 0) {
        if (++pos >= __argc - args) {
            return false;
        }
    }
    if (store != nullptr && args != 0) {
        do {
            store->push_back(std::string{__argv[++pos]});
            --args;
        } while (args);
    }
    return true;
}

// 0x0041B690
std::string Moho::CFG_GetArgs() {
    std::string builder{""};
    for (int i = 1; i < __argc; ++i) {
        builder.append(gpg::STR_Printf("%s ", __argv[i]));
    }
    return gpg::STR_Chop(builder.c_str(), '\0');
}

// 0x004091F0
int func_Dispatch(Moho::CTaskThread *thrd) {
    if (0 < --thrd->val1) {
        return 0;
    }
    while (true) {
        Moho::CTask *task = thrd->mTask;
        if (task == nullptr) {
            return -1;
        }
        bool v10 = false;
        task->mDestroyed = &v10;
        int res = task->Dispatch();
        if (! v10) {
            task->mDestroyed = nullptr;
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
                for (i = &thrd->mTask; *i != task; i = &(*i)->mSubtask)
                    {}
                *i = task->mSubtask;
                task->mSubtask = nullptr;
                task->mTaskThread = nullptr;
                if (task->mIsOwning) {
                    delete(task);
                }
                break;
            }
            case 0: {
                thrd->val1 = 0;
                if (thrd->mStaged) {
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
    Moho::TDatListItem<Moho::CTaskThread, void> *v2; // ecx
    Moho::TDatListItem<Moho::CTaskThread, void> *next; // esi
    int v4; // eax
    Moho::TDatListItem<Moho::CTaskThread, void> *v5; // edi
    Moho::TDatListItem<Moho::CTaskThread, void> *prev; // eax
    Moho::TDatListItem<Moho::CTaskThread, void> v7; // [esp+8h] [ebp-18h] BYREF
    int v8; // [esp+1Ch] [ebp-4h]

    v2 = &v7;
    Moho::TDatListItem<Moho::CTaskThread, void> v7;
    next = stage->mThreads.mNext;
    if (next != &stage->mThreads) {
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
                next->prev = stage->mThreads.prev;
                next->next = &stage->mThreads;
                stage->threads.prev = next;
                next->prev->next = next;
            } else if (v4 == -1) {
                delete(next)
            }
            next = stage->mThreads.next;
        } while (next != &stage->mThreads);
        v2 = v7.next;
    }
    v5 = stage->mThreads.next;
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
        sWakeupTimer.ElapsedCyclesAndReset()
    );
}

// 0x004F2000
bool func_HasCorrectPlatform() {
    _OSVERSIONINFOW info;
    ZeroMemory(&info, sizeof(info));
    info.dwOSVersionInfoSize = sizeof(info);
    return ! (GetVersionExW(&info) && info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
}

// 0x004F2050
LRESULT func_WindowHook(int code, WPARAM wParam, DWORD *lParam) {
    if (code == 0
        && Moho::WIN_GetCurrentApp()->HasFrame()
        && (wParam == 0x100 || wParam == 0x101)
        && (*lParam == '[' || *lParam == '\\')
    ) {
        return 1;
    } else {
        return CallNextHookEx(sWindowHook, code, wParam, (LPARAM)lParam);
    }
}
