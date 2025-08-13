#include "WinApp.h"
#include "core/Thread.h"
#include "gpgcore/utils/LogHistory.h"

// 0x008CD460
bool Moho::IWinApp::Func0() {
    return false;
}

// 0x004F1BA0
bool Moho::IWinApp::AppInitCommonServices() {
    Moho::THREAD_InitInvoke();
    gpg::EnableLogHistory(100);
    std::vector<std::string> vec{};
    if (Moho::CFG_GetArgOption("/log", 1, &vec)) {
        // ... setup log
    }
    gpg::REF_RegisterAllTypes();
    // ... setup `path`
    if (! Moho::DISK_SetupDataAndSearchPaths(std::string{"SupComDataPath.lua"}, path)) {
        gpg::Die("Failed to setup initial search path.");
    }
}

// 0x008CD470
bool Moho::IWinApp::HasFrame() {
    return false;
}

// 0x008CD360
Moho::IWinApp::IWinApp(const char *title, const char *subtitle) :
    mTitle{title},
    mSubtitle{subtitle},
    mExitValue{0}
{}
