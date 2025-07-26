#include "ScApp.h"
#include "gpggal/Device.h"

// 0x008D4B20
float struct_RollingFrameRates::median() {
    float arr[10];
    int pos = 0;
    for (int k = this->start; k != this->end; k = (k + 1) % 10) {
        arr[pos++] = this->vals[k];
    }
    std::sort(arr, &arr[pos]);
    return arr[pos / 2];
}

// 0x008CEDE0
bool CScApp::Main() {
    func_SetSSE2(Moho::CFG_GetArgOption("/sse2", 0, nullptr));
    v54 = v53;
    if (Moho::CFG_GetArgOption("/splash", 0, nullptr)) {
        boost::filesystem::path dir = Moho::DISK_GetLaunchDir() / "splash.png";
        Moho::WINX_InitSplash(dir.string());
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
        tagMONITORINFO mi;
        mi.cbSize = 40;
        GetMonitorInfoW(MonitorFromPoint(0LL, 1), &mi);
        if (mi.rcMonitor.right - mi.rcMonitor.left < Moho::wnd_MinCmdLineWidth
            || mi.rcMonitor.bottom - mi.rcMonitor.top < Moho::wnd_MinCmdLineHeight
        ) {
            LuaPlus::LuaState *state = Moho::USER_GetLuaState();
            std::string cap = Moho::Loc(state, "<LOC Engine0029>Inadequate monitor resolution (minimum is 1024x768.)");
            std::string title = Moho::Loc(state, "<LOC Engine0013>Unable to start.");
            MessageBoxW(0, gpg::STR_Utf8ToWide(title.c_str()), gpg::STR_Utf8ToWide(cap.c_str()), 0x10);
            return false;
        }
    }
    if (! func_CreateDevice(this)) {
        std::vector<std::string> log = gpg::gal::Device::GetInstance()->GetLog();
        std::string desc = Moho::Loc(Moho::USER_GetLuaState(), "<LOC Engine0018>Please verify video hardware meets minimum specifications.");
        if (log.size()) {
            desc = Moho::Loc(Moho::USER_GetLuaState(), log[0].c_str());
        }
        desc = desc + "\n" + Moho::Loc(Moho::USER_GetLuaState(), "<LOC Engine0019>Please ensure system has current video drivers.");
        std::string title = Moho::Loc(Moho::USER_GetLuaState(), "<LOC Engine0013>Unable to start.");
        MessageBoxW(0, gpg::STR_Utf8ToWide(title.c_str()), gpg::STR_Utf8ToWide(cap.c_str()), 0x10);
        return false;
    }
    if (Moho::CFG_GetArgOption("/loadrulesandexit", 0, nullptr)) {
        auto rules = new Moho::RRuleGameRules{std::string{}, nullptr};
        delete(rules);
        return false;
    }
    std::string init8{" SCMain AppInit 8"};
    SystemParametersInfoW(SPI_GETSCREENSAVEACTIVE, 0, &this->usingScreensaver, 0);
    if (! SystemParametersInfoW(SPI_SETSCREENSAVEACTIVE, 0, 0, 0)) {
        gpg::Warnf("unable to suppress screensaver");
    }
    gpg::gal::DeviceContext *context = gpg::gal::Device::GetInstance()->GetDeviceContext();
    if (context->GetHead(0)->windowed) {
        func_CreateFileMapping(context->GetHead(0)->handle);
    }
    if (! context->GetHead(0)->GetHead(0)->windowed && Moho::USER_DebugFacilitiesEnabled()) {
        func_AppendMenuLuaDebugger(context->GetHead(0)->handle);
    }
    Moho::OPTIONS_Apply();
    if (context->GetHeadCount() == 1 &&
        context->GetHead(0)->windowed &&
        Moho::OPTIONS_GetInt("lock_fullscreen_cursor_to_window") == 1
    ) {
        RECT r;
        GetWindowRect(this->supcomFrame->GetHandle(), &r);
        ClipCursor(&r);
    }
    std::string init{" SCMain AppInit 9"};
    return true;
}

// 0x008D1470
int CScApp::OnNoMoreEvents() {
    Moho::CTimeBarSection bar{"AppFrame"};
    struct_HeapStats heapStats = funcGetHeapInfo();
    if (engine_stat_heap_reserved == nullptr) {
        engine_stat_heap_reserved = func_GetEngineStats()->GetItem("Heap_Reserved");    
    }
    engine_stat_heap_reserved->SetValue(func_FmtByteSize(heapStats.reserved));
    
    if (engine_stat_heap_committed == nullptr) {
        engine_stat_heap_committed = func_GetEngineStats()->GetItem("Heap_Committed");    
    }
    engine_stat_heap_committed->SetValue(func_FmtByteSize(heapStats.committed));
    
    if (engine_stat_heap_total == nullptr) {
        engine_stat_heap_total = func_GetEngineStats()->GetItem("Heap_Total");    
    }
    engine_stat_heap_total->SetValue(func_FmtByteSize(heapStats.total));
    
    if (engine_stat_heap_insmallblocks == nullptr) {
        engine_stat_heap_insmallblocks = func_GetEngineStats()->GetItem("Heap_InSmallBlocks");    
    }
    engine_stat_heap_insmallblocks->SetValue(func_FmtByteSize(heapStats.inSmallBlocks));
    
    if (engine_stat_heap_inuse == nullptr) {
        engine_stat_heap_inuse = func_GetEngineStats()->GetItem("Heap_InUse");    
    }
    engine_stat_heap_inuse->SetValue(func_FmtByteSize(heapStats.inUse));
    
    if (engine_stat_heap_totalcheck == nullptr) {
        engine_stat_heap_totalcheck = func_GetEngineStats()->GetItem("Heap_TotalCheck");    
    }
    engine_stat_heap_totalcheck->SetValue(func_FmtByteSize(heapStats.inSmallBlocks + heapStats.inUse));

    if (dbg_MonitorAddressSpace) {
        static gpg::time::Timer monitor{};
        if (monitor.ElapsedSeconds() > 3.0) {
            monitor.Reset();
            func_QueryHeap();
        }
    }
    if (csimdriver != nullptr) {
        int hnd = csimdriver->GetEvent();
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
            float millis = this->curTime.ElapsedMilliseconds();
            if (Moho::sc_FrameTimeClamp > millis) {
                Moho::WIN_SetWakeupTimer(Moho::sc_FrameTimeClamp - millis);
                return;
            }
            framerate = millis * 0.001;
        }
    }
    this->curTime.Reset();
    this->framerates.roll(framerate);

    Moho::SND_Frame();
    func_CheckDiskWatch();
    float rate;
    if (Moho::dump_frame_rate) {
        rate = 1 / Moho::dump_rate;
    } else {
        rate = this->framerates.median();
    }
    if (! Moho::WLD_Frame(rate)) {
        wxTheApp->ExitMainLoop();
    }
    int tick = 0;
    int delta = 0.0;
    if (cwldsession != nullptr) {
        tick = cwldsession->gameTick;
        delta = cwldsession->timeSinceLastTick;
    }
    func_GetCamManager()->Frame(delta, framerate);
    func_UserFrame(pUserstage);
    ADXM_ExecMain();
    Moho::REN_Frame(tick, delta, framerate);
    Moho::USER_GetSound()->Frame(delta, framerate);
    if (global_CUIManager != nullptr) {
        global_CUIManager->Update(framerate);
    }
    bool isMinimized = IsIconic(mainWindow->GetHandle());
    if (isMinimized) {
        if (cwldsession != nullptr) {
            float delay = cwldsession->GetDelayToNextBeat();
            if (delay > 0.0) {
                Moho::WIN_SetWakeupTimer(delay * 1000.0);
            } else {
                int hnd = csimdriver->GetEvent();
                Moho::WIN_GetWaitHandleSet()->AddHandle(hnd);
            }
        }
    } else {
        Moho::D3D_GetDevice()->Refresh();
    }
    if (! this->initialized) {
        func_InitializeProcess();
        this->initialized = true;
    }
    if (this->isMinimized != isMinimized) {
        Moho::SND_Mute(isMinimized);
        if (global_CUIManager != nullptr) {
            global_CUIManager->Func4(&isMinimized);
        }
        gpg::Logf("Minimized %s", isMinimized ? "true" : "false");
    }
    this->isMinimized = isMinimized;
}

// 0x008D0F20
void CScApp::Destroy() {
    if (! SystemParametersInfoW(SPI_SETSCREENSAVEACTIVE, this->usingScreensaver, 0, 0)) {
        gpg::Warnf("unable to reset screensaver");
    }
    Moho::WLD_Teardown();
    func_NetCleanup();
    if (global_CUIManager != nullptr) {
        delete(global_CUIManager);
    }
    global_CUIManager = nullptr;
    func_GetWldSessionLoader()->Func6();
    if (moviemanager != nullptr) {
        moviemanager->Destroy();
    }
    moviemanager = nullptr;
    Moho::D3D_Exit();
    mainWindow = nullptr;
    ren_viewport = nullptr;
    if (soundconfiguration != nullptr) {
        delete(soundconfiguration);
    }
    Moho::SCR_DestroyDebugWindow();
    Moho::USER_SavePreferences();
    if (this->supcomFrame != nullptr) {
        this->supcomFrame->Destroy();
    }
    if (this->frame != nullptr) {
        this->frame->Destroy();
    }
    ClipCursor(0);
}

// 0x008CE1D0
bool CScApp::HasFrame() {
    return
        gpg::gal::Device::IsReady() &&
        gpg::gal::Device::GetInstance()->GetDeviceContext()->GetHead(0)->windowed &&
        this->supcomFrame != nullptr &&
        this->supcomFrame->v94d;
}
