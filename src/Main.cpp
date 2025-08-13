#include "ScApp.h"
#include "core/Plat.h"
#include "core/Win.h"
#include "user/User.h"
#include "gpgcore/General.h"
#include "gpgcore/Timer.h"
#include <Windows.h>
#include <string>
#include <vector>


using mem_hook_t = void (*)(int, int, ...);

bool sAQTime = true; // 0x00F3148C
STICKYKEYS sObjStickyKeys; // 0x00F57888
TOGGLEKEYS sObjToggleKeys; // 0x00F57890
FILTERKEYS sObjFilterKeys; // 0x00F57898
mem_hook_t sMemHook; // 0x00F8F330
RTL_CRITICAL_SECTION sLogCritSection; // 0x010A6344
FILE *sAlloclog; // 0x010A648C
bool sFlushingAlloclog; // 0x010A6490

void func_MemHook(int isFreeing, int size, ...); // 0x008D1E50
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
    std::vector<std::string> allocFilename;
    if (Moho::CFG_GetArgOption("/alloclog", 1, &allocFilename)) {
        func_AllocLog(allocFilename[0].c_str());
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


// 0x008D1E50
void func_MemHook(int isFreeing, int size, ...) {
    va_list ptrs;
    va_start(ptrs, size);
    EnterCriticalSection(&sLogCritSection);
    if (! sFlushingAlloclog) {
        sFlushingAlloclog = true;
        DWORD buffer = GetCurrentThreadId();
        fwrite(&buffer, 4u, 1u, sAlloclog);
        LARGE_INTEGER performanceCount;
        QueryPerformanceCounter(&performanceCount);
        fwrite(&performanceCount, 8, 1, sAlloclog);
        fwrite(&isFreeing, 4, 1, sAlloclog);
        fwrite(&size, 4, 1, sAlloclog);
        fwrite(ptrs, 4, 1, sAlloclog);
        // see Moho::PLAT_FormatCallStack
        unsigned int stack[64];
        unsigned int stackSize = Moho::PLAT_GetCallStack(0, 64, stack);
        unsigned int *addr = &stack[0];
        for ( ; stackSize != 0; --stackSize, ++addr) {
            fwrite(addr, 4u, 1u, sAlloclog);
            if (sub_8D4C10(addr)->found) {
                Moho::SPlatSymbolInfo info{};
                std::string out{};
                if (Moho::PLAT_GetSymbolInfo(*addr, info)) {
                    out = gpg::STR_Printf(
                        "%s + %d bytes (%s(%d) + %d bytes)",
                        info.mSymbol.c_str(),
                        info.mSymDis,
                        info.mFilename.c_str(),
                        info.mLineNum,
                        info.mLineDis
                    );
                }
                fwrite(out.c_str(), 1, out.size() + 1, sAlloclog);
            }
        }
        int zero = 0;
        fwrite(&zero, 4, 1, sAlloclog);
        sFlushingAlloclog = false;
    }
    LeaveCriticalSection(&sLogCritSection);
}

// 0x008D2140
void func_CleanupAllocLoc() {
    func_SetMemHook(0);
    fclose(sAlloclog);
    sAlloclog = nullptr;
    DeleteCriticalSection(&SleepEx);
}

// 0x008D2170
void func_AllocLog(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        fwrite(&frequency, 8, 1, file);
        InitializeCriticalSection(&sLogCritSection);
        sAlloclog = file;
        atexit(func_CleanupAllocLoc);
        func_SetMemHook(func_MemHook);
    }
}

// 0x008D4320
void func_SetSystemParameters(bool force) {
    if (force) {
        SystemParametersInfoW(SPI_SETSTICKYKEYS, sizeof(sObjStickyKeys), &sObjStickyKeys, 0);
        SystemParametersInfoW(SPI_SETTOGGLEKEYS, sizeof(sObjToggleKeys), &sObjToggleKeys, 0);
        SystemParametersInfoW(SPI_SETFILTERKEYS, sizeof(sObjFilterKeys), &sObjFilterKeys, 0);
    } else {
        STICKYKEYS stickykeys = sObjStickyKeys;
        if ((sObjStickyKeys.dwFlags & SKF_STICKYKEYSON) == 0) {
            togglekeys.dwFlags &=~ (SKF_HOTKEYACTIVE|SKF_CONFIRMHOTKEY);
            SystemParametersInfoW(SPI_SETSTICKYKEYS, sizeof(stickykeys), &stickykeys, 0);
        }
        TOGGLEKEYS togglekeys = sObjToggleKeys;
        if ((sObjToggleKeys.dwFlags & TKF_TOGGLEKEYSON) == 0) {
            togglekeys.dwFlags &=~ (TKF_HOTKEYACTIVE|TKF_CONFIRMHOTKEY);
            SystemParametersInfoW(SPI_SETTOGGLEKEYS, sizeof(togglekeys), &togglekeys, 0);
        }
        FILTERKEYS filterkeys = sObjFilterKeys;
        if ((sObjFilterKeys.dwFlags & FKF_FILTERKEYSON) == 0) {
            filterkeys.dwFlags &=~ (FKF_HOTKEYACTIVE|FKF_CONFIRMHOTKEY);
            SystemParametersInfoW(SPI_SETFILTERKEYS, sizeof(filterkeys), &filterkeys, 0);
        }
    }
}

// 0x008D4410
bool func_CheckMediaCenter() {
    WCHAR filename[260];
    return Moho::CFG_GetArgOption("/mediacenter", 0, nullptr)
        && GetSystemMetrics(SM_MEDIACENTER)
        && ExpandEnvironmentStringsW(L"%SystemRoot%\\ehome\\ehshell.exe", filename, sizeof(filename))
        && GetFileAttributesW(filename) != -1
        && (int) ShellExecuteW(0, L"open", filename, 0, 0, true) > 32;
}

// 0x009071C0
void func_SetAqtime(bool val) {
    sAQTime = val;
}

// 0x00957EF0
void func_SetMemHook(mem_hook_t hook) {
    sMemHook = hook;
}
