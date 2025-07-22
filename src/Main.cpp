#include <Windows.h>
#include <string>
#include <vector>
#include "gpgcore/Timer.h"
#include "gpgcore/General.h"
#include "core/Win.h"
#include "App.h"

STICKYKEYS obj_stickykeys; // 0x00F57888
TOGGLEKEYS obj_togglekeys; // 0x00F57890
FILTERKEYS obj_filterkeys; // 0x00F57898

// 0x008D44A0
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char *cmdline, int nShowCmd) {
    gpg::time::Timer startTime{};
    if (Moho::CFG_GetArgOption(0, 0, "/waitfordebugger")) {
        MessageBoxW(0, L"Attach the debugger and click OK.", L"Waiting", 0);
    }
    if (Moho::CFG_GetArgOption(0, 0, "/aqtime")) {
        func_SetAqtime(0);
    }
    std::vector<std::string> v17;
    if (Moho::CFG_GetArgOption(1, &v17, "/alloclog")) {
        func_AllocLog(v17[0].c_str());
    }
    gpg::SetDieHandler(Moho::WIN_CrashDialogDieHandler);
    if (Moho::CFG_GetArgOption(0, 0, "/singleproc")) {
        unsigned long long process, system;
        GetProcessAffinityMask(GetCurrentProcess(), &process, &system);
        DWORD v8;
        _BitScanForward(&v8, process);
        SetProcessAffinityMask(GetCurrentProcess(), (1 << v8));
    }
    if (Moho::CFG_GetArgOption(0, 0, "/purgecache")) {
        Moho::USER_PurgeAppCacheDir();
    }
    SystemParametersInfoW(SPI_GETSTICKYKEYS, sizeof(obj_stickykeys), &obj_stickykeys, 0);
    SystemParametersInfoW(SPI_GETTOGGLEKEYS, sizeof(obj_togglekeys), &obj_togglekeys, 0);
    SystemParametersInfoW(SPI_GETFILTERKEYS, sizeof(obj_filterkeys), &obj_filterkeys, 0);
    func_WritebackSystemParameters(false);
    CScApp app{};
    Moho::WIN_AppExecute(&app);
    int runtime = (int) startTime.ElapsedSeconds();
    gpg::Logf("Run time: %dh%02dm%02ds", runtime / 3600, runtime % 3600 / 60, runtime % 60);
    func_WritebackSystemParameters(true);
    func_CheckMediaCenter();
    return app.exitValue;
}
