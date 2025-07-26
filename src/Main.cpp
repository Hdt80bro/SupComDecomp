#include <string>
#include <vector>
#include <Windows.h>
#include "gpgcore/General.h"
#include "gpgcore/Timer.h"
#include "core/App.h"
#include "core/Win.h"
#include "user/User.h"
#include "ScApp.h"


using mem_hook_t = void (*)(int, int, ...);

bool aqtime = true; // 0x00F3148C
STICKYKEYS obj_stickykeys; // 0x00F57888
TOGGLEKEYS obj_togglekeys; // 0x00F57890
FILTERKEYS obj_filterkeys; // 0x00F57898
mem_hook_t mem_hook; // 0x00F8F330
FILE *alloclog; // 0x
RTL_CRITICAL_SECTION logCritSection; // 0x

void func_CleanupAllocLoc(); // 0x008D2140
void func_AllocLog(const char *); // 0x008D2170
void func_SetSystemParameters(bool force); // 0x008D4320
bool func_CheckMediaCenter(); // 0x008D4410
void func_SetAqtime(bool); // 0x009071C0
void func_SetMemHook(mem_hook_t hook); // 0x00957EF0


// 0x008D44A0
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char *cmdline, int nShowCmd) {
    gpg::time::Timer startTime{};
    if (Moho::CFG_GetArgOption("/waitfordebugger", 0, nullptr)) {
        MessageBoxW(0, L"Attach the debugger and click OK.", L"Waiting", 0);
    }
    if (Moho::CFG_GetArgOption("/aqtime", 0, nullptr)) {
        func_SetAqtime(0);
    }
    std::vector<std::string> v17;
    if (Moho::CFG_GetArgOption("/alloclog", 1, &v17)) {
        func_AllocLog(v17[0].c_str());
    }
    gpg::SetDieHandler(Moho::WIN_CrashDialogDieHandler);
    if (Moho::CFG_GetArgOption("/singleproc", 0, nullptr)) {
        unsigned long long process, system;
        GetProcessAffinityMask(GetCurrentProcess(), &process, &system);
        DWORD v8;
        _BitScanForward(&v8, process);
        SetProcessAffinityMask(GetCurrentProcess(), (1 << v8));
    }
    if (Moho::CFG_GetArgOption("/purgecache", 0, nullptr)) {
        Moho::USER_PurgeAppCacheDir();
    }
    SystemParametersInfoW(SPI_GETSTICKYKEYS, sizeof(obj_stickykeys), &obj_stickykeys, 0);
    SystemParametersInfoW(SPI_GETTOGGLEKEYS, sizeof(obj_togglekeys), &obj_togglekeys, 0);
    SystemParametersInfoW(SPI_GETFILTERKEYS, sizeof(obj_filterkeys), &obj_filterkeys, 0);
    func_SetSystemParameters(false);
    CScApp app{};
    Moho::WIN_AppExecute(&app);
    int runtime = (int) startTime.ElapsedSeconds();
    gpg::Logf("Run time: %dh%02dm%02ds", runtime / 3600, runtime % 3600 / 60, runtime % 60);
    func_SetSystemParameters(true);
    func_CheckMediaCenter();
    return app.exitValue;
}


// 0x008D2140
void func_CleanupAllocLoc() {
    func_SetMemHook(0);
    fclose(alloclog);
    alloclog = 0;
    DeleteCriticalSection(&logCritSection);
}

// 0x008D2170
void func_AllocLog(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        fwrite(&frequency, 8u, 1u, file);
        InitializeCriticalSection(&logCritSection);
        alloclog = file;
        atexit(func_CleanupAllocLoc);
        func_SetMemHook(func_MemHook);
    }
}

// 0x008D4320
void func_SetSystemParameters(bool force) {
    if (force) {
        SystemParametersInfoW(SPI_SETSTICKYKEYS, sizeof(obj_stickykeys), &obj_stickykeys, 0);
        SystemParametersInfoW(SPI_SETTOGGLEKEYS, sizeof(obj_togglekeys), &obj_togglekeys, 0);
        SystemParametersInfoW(SPI_SETFILTERKEYS, sizeof(obj_filterkeys), &obj_filterkeys, 0);
    } else {
        STICKYKEYS stickykeys = obj_stickykeys;
        if ((obj_stickykeys.dwFlags & SKF_STICKYKEYSON) == 0) {
            stickykeys.dwFlags &=~ (SKF_HOTKEYACTIVE|SKF_CONFIRMHOTKEY);
            SystemParametersInfoW(SPI_SETSTICKYKEYS, sizeof(stickykeys), &stickykeys, 0);
        }
        TOGGLEKEYS togglekeys = obj_togglekeys;
        if ((obj_togglekeys.dwFlags & TKF_TOGGLEKEYSON) == 0) {
            togglekeys.dwFlags &=~ (TKF_HOTKEYACTIVE|TKF_CONFIRMHOTKEY);
            SystemParametersInfoW(SPI_SETTOGGLEKEYS, sizeof(togglekeys), &togglekeys, 0);
        }
        FILTERKEYS filterkeys = obj_filterkeys;
        if ((obj_filterkeys.dwFlags & FKF_FILTERKEYSON) == 0) {
            filterkeys.dwFlags &=~ (FKF_HOTKEYACTIVE|FKF_CONFIRMHOTKEY);
            SystemParametersInfoW(SPI_SETFILTERKEYS, sizeof(filterkeys), &filterkeys, 0);
        }
    }
}

// 0x008D4410
bool func_CheckMediaCenter() {
    WCHAR filename[260];
    return Moho::CFG_GetArgOption(0, 0, "/mediacenter")
        && GetSystemMetrics(SM_MEDIACENTER)
        && ExpandEnvironmentStringsW(L"%SystemRoot%\\ehome\\ehshell.exe", filename, sizeof(filename))
        && GetFileAttributesW(filename) != -1
        && (int) ShellExecuteW(0, L"open", filename, 0, 0, true) > 32;
}

// 0x009071C0
void func_SetAqtime(bool val) {
    aqtime = val;
}

// 0x00957EF0
void func_SetMemHook(mem_hook_t hook) {
    mem_hook = hook;
}
