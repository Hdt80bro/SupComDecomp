#include "ScApp.h"
#include "core/Disk.h"
#include "user/User.h"
#include "gpgcore/String.h"
#include "gpggal/Device.h"
#include "LuaPlus.h"
#include "boost/filesystem/path.hpp"
#include <string>
#include <vector>

// 0x008D4B20
float struct_RollingFrameRates::median() {
    float arr[10];
    int pos = 0;
    for (int k = this->mStart; k != this->mEnd; k = (k + 1) % 10) {
        arr[pos++] = this->mVals[k];
    }
    std::sort(arr, &arr[pos]);
    return arr[pos / 2];
}

// 0x008CEDE0
bool CScApp::AppInit() {
    func_SetSSE2(Moho::CFG_GetArgOption("/sse2", 0, nullptr));
    if (Moho::CFG_GetArgOption("/splash", 0, nullptr)) {
        boost::filesystem::path dir = Moho::DISK_GetLaunchDir() / "splash.png";
        Moho::WINX_InitSplash(dir.string().c_str());
    }
    std::string init1{" SCMain AppInit 1"};
    if (! this->AppInitCommonServices()) {
        return false;
    }
    std::string init2{" SCMain AppInit 2"};
    std::vector<std::string> args{};
    if (Moho::CFG_GetArgOption("/prefs", 1, &args)) {
        Moho::USER_LoadPreferences(&args[0]);
    } else {
        Moho::USER_LoadPreferences(std::string{"Game.prefs"});
    }
    std::string init3{" SCMain AppInit 3"};
    Moho::SCR_LuaDoScript(Moho::USER_GetLuaState(), "/lua/userInit.lua", nullptr);
    std::string init3{" SCMain AppInit 4"};
    func_OpenDocuments();
    if (Moho::CFG_GetArgOption("/profile", 1, &args)) {
        Moho::USER_SetCurrentProfile(&args[0]);
    }
    Moho::USER_DebugFacilitiesEnabled();
    std::string init5{" SCMain AppInit 5"};
    Moho::REN_Init();
    std::string init6{" SCMain AppInit 6"};
    Moho::UI_Init();
    std::string init7{" SCMain AppInit 7"};
    if (! Moho::CFG_GetArgOption("/novalidate", 0, nullptr)) {
        MONITORINFO mi;
        mi.cbSize = 40;
        GetMonitorInfoW(MonitorFromPoint(0LL, 1), &mi);
        if (mi.rcMonitor.right - mi.rcMonitor.left < Moho::wnd_MinCmdLineWidth
            || mi.rcMonitor.bottom - mi.rcMonitor.top < Moho::wnd_MinCmdLineHeight
        ) {
            LuaPlus::LuaState *state = Moho::USER_GetLuaState();
            std::string cap = Moho::Loc(state, "<LOC Engine0029>Inadequate monitor resolution (minimum is 1024x768.)");
            std::string title = Moho::Loc(state, "<LOC Engine0013>Unable to start.");
            MessageBoxW(0, gpg::STR_Utf8ToWide(title.c_str()), gpg::STR_Utf8ToWide(cap.c_str()), MB_ICONSTOP);
            return false;
        }
    }
    if (! this->CreateDevice()) {
        std::vector<std::string> log = gpg::gal::Device::GetInstance()->GetLog();
        std::string desc = Moho::Loc(Moho::USER_GetLuaState(), "<LOC Engine0018>Please verify video hardware meets minimum specifications.");
        if (log.size() > 0) {
            desc = Moho::Loc(Moho::USER_GetLuaState(), log[0].c_str());
        }
        desc = desc + "\n" + Moho::Loc(Moho::USER_GetLuaState(), "<LOC Engine0019>Please ensure system has current video drivers.");
        std::string title = Moho::Loc(Moho::USER_GetLuaState(), "<LOC Engine0013>Unable to start.");
        MessageBoxW(0, gpg::STR_Utf8ToWide(title.c_str()).c_str(), gpg::STR_Utf8ToWide(desc.c_str()).c_str(), MB_ICONSTOP);
        return false;
    }
    if (Moho::CFG_GetArgOption("/loadrulesandexit", 0, nullptr)) {
        auto rules = new Moho::RRuleGameRules{std::string{}, nullptr};
        delete(rules);
        return false;
    }
    std::string init8{" SCMain AppInit 8"};
    SystemParametersInfoW(SPI_GETSCREENSAVEACTIVE, 0, &this->mUsingScreensaver, 0);
    if (! SystemParametersInfoW(SPI_SETSCREENSAVEACTIVE, 0, 0, 0)) {
        gpg::Warnf("unable to suppress screensaver");
    }
    gpg::gal::DeviceContext *context = gpg::gal::Device::GetInstance()->GetDeviceContext();
    if (context->GetHead(0)->mWindowed) {
        func_CreateFileMapping(context->GetHead(0)->mHandle);
    }
    if (! context->GetHead(0)->mWindowed && Moho::USER_DebugFacilitiesEnabled()) {
        func_AppendMenuLuaDebugger(context->GetHead(0)->mHandle);
    }
    Moho::OPTIONS_Apply();
    if (context->GetHeadCount() == 1 &&
        context->GetHead(0)->mWindowed &&
        Moho::OPTIONS_GetInt("lock_fullscreen_cursor_to_window") == 1
    ) {
        RECT r;
        GetWindowRect(this->mFrame1->GetHandle(), &r);
        ClipCursor(&r);
    }
    std::string init{" SCMain AppInit 9"};
    return true;
}

// 0x008D1470
int CScApp::Main() {
    Moho::CTimeBarSection bar{"AppFrame"};
    struct_HeapStats heapStats = funcGetHeapInfo();
    if (sEngineStat_Heap_Reserved == nullptr) {
        sEngineStat_Heap_Reserved = func_GetEngineStats()->GetItem("Heap_Reserved");
    }
    sEngineStat_Heap_Reserved->SetValue(func_FmtByteSize(heapStats.mReserved));
    
    if (sEngineStat_Heap_Committed == nullptr) {
        sEngineStat_Heap_Committed = func_GetEngineStats()->GetItem("Heap_Committed");
    }
    sEngineStat_Heap_Committed->SetValue(func_FmtByteSize(heapStats.mCommitted));
    
    if (sEngineStat_Heap_Total == nullptr) {
        sEngineStat_Heap_Total = func_GetEngineStats()->GetItem("Heap_Total");
    }
    sEngineStat_Heap_Total->SetValue(func_FmtByteSize(heapStats.mTotal));
    
    if (sEngineStat_Heap_InSmallBlocks == nullptr) {
        sEngineStat_Heap_InSmallBlocks = func_GetEngineStats()->GetItem("Heap_InSmallBlocks");
    }
    sEngineStat_Heap_InSmallBlocks->SetValue(func_FmtByteSize(heapStats.mInSmallBlocks));
    
    if (sEngineStat_Heap_InUse == nullptr) {
        sEngineStat_Heap_InUse = func_GetEngineStats()->GetItem("Heap_InUse");
    }
    sEngineStat_Heap_InUse->SetValue(func_FmtByteSize(heapStats.mInUse));
    
    if (sEngineStat_Heap_TotalCheck == nullptr) {
        sEngineStat_Heap_TotalCheck = func_GetEngineStats()->GetItem("Heap_TotalCheck");
    }
    sEngineStat_Heap_TotalCheck->SetValue(func_FmtByteSize(heapStats.mInSmallBlocks + heapStats.mInUse));

    if (dbg_MonitorAddressSpace) {
        static gpg::time::Timer monitor{};
        if (monitor.ElapsedSeconds() > 3.0) {
            monitor.Reset();
            func_QueryHeap();
        }
    }
    if (sSimDriver != nullptr) {
        int hnd = sSimDriver->GetEvent();
        Moho::WIN_GetWaitHandleSet()->RemoveHandle(hnd);
    }
    float framerate;
    if (Moho::dump_frame_rate) {
        framerate = 1.0 / Moho::dump_Rate;
    } else if (this->v21) {
        framerate = 0.0;
        this->v21 = false;
    } else {
        std::vector<std::string> args{};
        float clfr;
        if (Moho::CFG_GetArgOption("/framerate", 1, &args)
            && (clfr = atof(args[0].c_str()) > 0.0)
        ) {
            framerate = 1.0 / clfr;
        } else {
            float millis = this->mCurTime.ElapsedMilliseconds();
            if (Moho::sc_FrameTimeClamp > millis) {
                Moho::WIN_SetWakeupTimer(Moho::sc_FrameTimeClamp - millis);
                return;
            }
            framerate = millis * 0.001;
        }
    }
    this->mCurTime.Reset();
    this->mFrameRates.roll(framerate);

    Moho::SND_Frame();
    func_CheckDiskWatch();
    float rate;
    if (Moho::dump_frame_rate) {
        rate = 1 / Moho::dump_rate;
    } else {
        rate = this->mFramerates.median();
    }
    if (! Moho::WLD_Frame(rate)) {
        wxTheApp->ExitMainLoop();
    }
    int tick = 0;
    int delta = 0.0;
    if (sWldSession != nullptr) {
        tick = sWldSession->mGameTick;
        delta = sWldSession->mTimeSinceLastTick;
    }
    func_GetCamManager()->Frame(delta, framerate);
    func_UserFrame(sPUserStage);
    ADXM_ExecMain();
    Moho::REN_Frame(tick, delta, framerate);
    Moho::USER_GetSound()->Frame(delta, framerate);
    if (sUIManager != nullptr) {
        sUIManager->Update(framerate);
    }
    bool isMinimized = IsIconic(mainWindow->GetHandle());
    if (isMinimized) {
        if (sWldSession != nullptr) {
            float delay = sWldSession->GetDelayToNextBeat();
            if (delay > 0.0) {
                Moho::WIN_SetWakeupTimer(delay * 1000.0);
            } else {
                int hnd = sSimDriver->GetEvent();
                Moho::WIN_GetWaitHandleSet()->AddHandle(hnd);
            }
        }
    } else {
        Moho::D3D_GetDevice()->Refresh();
    }
    if (! this->mInitialized) {
        func_InitializeSession();
        this->mInitialized = true;
    }
    if (this->mIsMinimized != isMinimized) {
        Moho::SND_Mute(isMinimized);
        if (sUIManager != nullptr) {
            sUIManager->Func4(&isMinimized);
        }
        gpg::Logf("Minimized %s", isMinimized ? "true" : "false");
    }
    this->mIsMinimized = isMinimized;
}

// 0x008D0F20
void CScApp::Destroy() {
    if (! SystemParametersInfoW(SPI_SETSCREENSAVEACTIVE, this->usingScreensaver, 0, 0)) {
        gpg::Warnf("unable to reset screensaver");
    }
    Moho::WLD_Teardown();
    func_NetCleanup();
    if (sUIManager != nullptr) {
        delete(sUIManager);
    }
    sUIManager = nullptr;
    func_GetWldSessionLoader()->Func6();
    if (sMovieManager != nullptr) {
        sMovieManager->Destroy();
    }
    sMovieManager = nullptr;
    Moho::D3D_Exit();
    sMainWindow = nullptr;
    ren_viewport = nullptr;
    if (sSoundConfiguration != nullptr) {
        delete(sSoundConfiguration);
    }
    Moho::SCR_DestroyDebugWindow();
    Moho::USER_SavePreferences();
    if (this->mFrame1 != nullptr) {
        this->mFrame1->Destroy();
    }
    if (this->mFrame2 != nullptr) {
        this->mFrame2->Destroy();
    }
    ClipCursor(0);
}

// 0x008CE1D0
bool CScApp::HasFrame() {
    return
        gpg::gal::Device::IsReady() &&
        gpg::gal::Device::GetInstance()->GetDeviceContext()->GetHead(0)->mWindowed &&
        this->mFrame1 != nullptr &&
        this->mFrame1->v94d;
}
