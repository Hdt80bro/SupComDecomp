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
    sRenViewport = nullptr;
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

// 0x008CF8C0
bool CScApp::CreateAppFrame(std::string *title, BOOL maximized, wxPoint *pos, gpg::gal::DeviceContext *context) {
    gpg::gal::Head *head = context->GetHead(0);
    int style = head->mWindowed ? wxNO_BORDER|wxSYSTEM_MENU : wxDEFAULT_FRAME_STYLE;
    wxSize size{head->mWidth, head->mHeight};
    WSupComFrame *frame = new WSupComFrame{title->c_str(), pos, &size, style};
    this->mSupComFrame = frame;
    if (head->mWindowed) {
        frame->Show(false);
    } else {
        if (maximized) {
            frame->Maximize(true);
        } else {
            frame->DoSetClientSize(size.x, size.y);
            int w, h;
            this->mSupComFrame->DoGetClientSize(&w, &h);
            if (w != size.x || h != size.y) {
                gpg::Warnf("Unable to set requested size %i,%i. Results are undefined.", size.x, size.y);
            }
        }
        if (! Moho::CFG_GetArgOption("/splash", 0, nullptr)) {
            delete(sSplashScreenPtr);
            sSplashScreenPtr = nullptr;
        }
        this->mSupComFrame->Show(true);
    }
    if (! head->mWindowed && maximized) {
        int w, h;
        this->mSupComFrame->DoGetClientSize(&w, &h);
        size.x = w;
        size.y = h;
        head->mWidth = size.x;
        head->mHeight = size.y;
    }
    sMainWindow = this->mSupComFrame;
    int w, h;
    sMainWindow->DoGetClientSize(&w, &h);
    wxSize view{w, h};
    Moho::WRenViewport *viewport = Moho::REN_CreateGameViewport(&view, sMainWindow, title.c_str(), context->GetHeadCount() > 1);
    head->mHandle = viewport->m_parent->GetHandle();
    head->mWindow = viewport->GetHandle();
    if (context->GetHeadCount() > 1) {
        gpg::gal::Head *secondary = context->GetHead(1);
        wxSize secondarySize{secondary->mWidth, secondary->mHeight};
        WSupComFrame *wframe = new WSupComFrame{title.c_str(), pos, &secondarySize, style};
        this->mFrame = wframe;
        wframe->Show(! secondary->mWindowed);
        secondary->mHandle = head->mHandle;
        secondary->mWindow = this->mFrame->GetHandle();
    }
    viewport->OnPaint(wxPaintEvent{0});
    Moho::CD3DDevice *device = Moho::D3D_GetDevice();
    device->Clear2(1);
    context->mValidate = !Moho::CFG_GetArgOption("/novalidate", 0, nullptr);
    func_CreateDeviceD3D(context);
    device->SetRenViewport(context);
    device->Clear2(0);
    if (sSplashScreenPtr != nullptr) {
        delete(sSplashScreenPtr);
    }
    sSplashScreenPtr = nullptr;
    this->mSupComFrame->Show(true);
    if (this->mFrame != nullptr) {
        this->mFrame->Show(true);
    }
    sRenViewport = viewport;
    viewport->SetFocus();
    Moho::WINX_PrecreateLogWindow();
    if (sUIManager != nullptr) {
        sUIManager->Func1(viewport, this->mSupComFrame);
    }
    if (this->mFrame != nullptr && sUIManager != nullptr) {
        sUIManager->Func1(this->mFrame, this->mFrame);
    }
    return false;
}

// 0x008D0370
int CScApp::CreateDevice() {
    sDeviceLock = true;
    std::string::string title{"Forged Alliance"};
    std::vector<std::string> options{};
    Moho:IUserPrefs *prefs = Moho::USER_GetPreferences();
    Moho::d3d_WindowsCursor = Moho::CFG_GetArgOption("/D3D10", 0, nullptr);
    gpg::gal::DeviceContext context{Moho::CFG_GetArgOption("/D3D10", 0, nullptr) + 1};
    context.mVSync = Moho::OPTIONS_GetInt("vsync") == 1;
    context.AddHead(gpg::gal::Head{});
    gpg::gal::Head *head = context.GetHead(0);
    head->mWindowed = true;
    head->mWidth = Moho::wnd_DefaultCreateWidth;
    head->mHeight = Moho::wnd_DefaultCreateHeight;
    head->mFramesPerSecond = 60;
    wxPoint pos = wxDefaultPosition;
    bool isWindowed = false;
    bool secondary = false;
    if (func_ValidWindowOption(options)) {
        context.AddHead(gpg::gal::Head{});
        gpg::gal::Head *head0 = context.GetHead(0);
        gpg::gal::Head *head1 = context.GetHead(1);
        head0->mWindowed = true;
        head1->mWindowed = true;
        int optW = atoi(options[0].c_str());
        head0->mWidth = MAX(optW, Moho::wnd_DefaultCreateWidth);
        int optH = atoi(options[1].c_str());
        head0->mHeight = MAX(optH, Moho::wnd_DefaultCreateHeight);
        if (options.size() == 4) {
            int optW = atoi(options[2].c_str());
            head1->mWidth = MAX(optW, Moho::wnd_DefaultCreateWidth);
            int optH = atoi(options[3].c_str());
            head1->mHeight = MAX(optH, Moho::wnd_DefaultCreateHeight);
            sAdapterNotCLOverridden = false;
        } else {
            head1->mWidth = head0->mWidth;
            head1->mHeight = head0->mHeight;
            sAdapterNotCLOverridden = false;
        }
    } else if (func_ValidFullscreenArg(&options)) {
        gpg::gal::Head *head = context.GetHead(0);
        head->mWindowed = true;
        int optW = atoi(options[0].c_str());
        head->mWidth = MAX(optW, Moho::wnd_DefaultCreateWidth);
        int optH = atoi(options[1].c_str());
        head->mHeight = MAX(optH, Moho::wnd_DefaultCreateHeight);
        sAdapterNotCLOverridden = false;
    } else {
        if (func_ValidWindowedOption(&options)) {
            gpg::gal::Head *head = context.GetHead(0);
            head->mWindowed = false;
            int optW = atoi(options[0].c_str());
            head->mWidth = MAX(optW, Moho::wnd_MinCmdLineWidth);
            int optH = atoi(options[1].c_str());
            head->mHeight = MAX(optH, Moho::wnd_MinCmdLineHeight);
            isWindowed = true;
            sAdapterNotCLOverridden = false;
        } else if (Moho::OPTIONS_GetString("secondary_adapter").compare("disabled") != 0
                && Moho::OPTIONS_GetString("primary_adapter").compare("windowed") != 0
        ) {
            context.AddHead(gpg::gal::Head{});
            gpg::gal::Head head0 = context.GetHead(0);
            gpg::gal::Head head1 = context.GetHead(1);
            head0->mWindowed = true;
            head1->mWindowed = true;
            Resolution res0{Moho::OPTIONS_GetString("primary_adapter")};
            head0->mWidth = res0.mWidth;
            head0->mHeight = res0.mHeight;
            head0->mFramesPerSecond = res0.mFramesPerSecond;
            Resolution res1{Moho::OPTIONS_GetString("secondary_adapter")};
            head1->mWidth = res1.mWidth;
            head1->mHeight = res1.mHeight;
            head1->mFramesPerSecond = res1.mFramesPerSecond;
        } else if (Moho::OPTIONS_GetString("primary_adapter").compare("windowed") != 0) {
            Resolution res{Moho::OPTIONS_GetString("primary_adapter")};
            gpg::gal::Head *head = context.GetHead(0);
            head->mWidth = res.mWidth;
            head->mHeight = res.mHeight;
            head->mFramesPerSecond = res.mFramesPerSecond;
        } else {
            gpg::gal::Head *head = context.GetHead(0);
            head->mWindowed = false;
            head->mWidth = prefs->GetInteger(std::string{"Windows.Main.width"}, Moho::wnd_DefaultCreateWidth);
            head->mHeight = prefs->GetInteger(std::string{"Windows.Main.height"}, Moho::wnd_DefaultCreateHeight);
        }
        secondary = true;
    }
    if (context.GetHeadCount() == 1) {
        if (func_FindOptionAmong(0, &str_adapter, 1, &options)) {
            context.mAdapter = atoi(options[0].c_str());
        }
    }
    bool clMaximize = func_CLHasMaximize();
    sAdapterNotCLOverridden &= (clMaximize != 0) - 1;
    bool maximized = ! isWindowed
        && ! context.GetHead(0)->windowed
        && (clMaximize || prefs->GetBoolean(std::string{"Windows.Main.maximized"}, 0)));
    if (! context.GetHead(0)->mWindowed) {
        if (Moho::CFG_GetArgOption("/position", 2, &options)) {
            pos.x = atoi(options[0].c_str());
            pos.y = atoi(options[1].c_str());
        } else {
            pos.x = prefs->GetInteger(std::string{"Windows.Main.x"}, pos.x);
            pos.y = prefs->GetInteger(std::string{"Windows.Main.y"}, pos.y);
        }
        HMONITOR mon = MonitorFromPoint(pos, 2u);
        MONITORINFO moninfo;
        moninfo.cbSize = 40; // sizeof(moninfo) ?
        GetMonitorInfoW(mon, *moninfo);
        if (pos.x >= moninfo.rcMonitor.right) {
            pos.x = moninfo.rcMonitor.right;
        }
        if (pos.x < moninfo.rcMonitor.left) {
            pos.x = moninfo.rcMonitor.left;
        }
        if (pos.y >= moninfo.rcMonitor.bottom) {
            pos.y = moninfo.rcMonitor.bottom;
        }
        if (pos.y < moninfo.rcMonitor.top) {
            pos.y = moninfo.rcMonitor.top;
        }
    }
    int aa = Moho::OPTIONS_GetInt("antialiasing");
    gpg::gal::Head *head0 = context.GetHead(0);
    int aaHigh = aa >> 5;
    int aaLow = aa & 0x1F;
    head0->antialiasingHigh = aaHigh;
    head0->antialiasingLow = aaLow;
    if (context.GetHeadCount() > 1) {
        gpg::gal::Head *head1 = context.GetHead(1);
        head1->mAntialiasingHigh = aaHigh;
        head1->mAntialiasingLow = aaLow;
    }
    if (! sApp->CreateAppFrame(&title, maximized, &pos, &context)) {
        context = gpg::gal::DeviceContext{Moho::CFG_GetArgOption("/D3D10", 0, nullptr) + 1};
        gpg::gal::Head head{};
        head.mHeight = Moho::wnd_DefaultCreateHeight;
        head.mWindowed = true;
        head.mHidth = Moho::wnd_DefaultCreateWidth;
        context.AddHead(&head);
        if (! sApp->CreateAppFrame(&title, 0, &pos, &context)) {
            return 0;
        }
    }
    gpg::gal::Device::GetInstance()->GetDeviceContext(); // ?
    func_SetupPrimaryAdapterSettings();
    func_SetupSecondaryAdapterSettings(secondary);
    func_CreateFidelityPresets(); // patched out
    func_SetupFidelitySettings();
    func_SetupShadowQualitySettings();
    func_SetupAntiAliasingSettings();
    func_SetupBasicMovieManager();
    sDeviceLock = false;
    return 1;
}

// 0x008CE220
std::string func_FindMapScenario(const char *mapName) {
    if (mapName == nullptr || mapName[0] == '\0' || strchr(mapName, '/') != nullptr || strchr(mapName, '\\') != nullptr) {
        return std::string{mapName};
    } else {
        return gpg::STR_Printf("/maps/%s/%s_scenario.lua", mapName, mapName);
    }
}

// 0x008CE2A0
bool func_StartCommandLineSession(const char *mapName, bool isPerfTest) {
    LuaPlus::LuaObjectVoid module = Moho::SCR_Import(Moho::USER_GetLuaState(), "/lua/SinglePlayerLaunch.lua");
    LuaPlus::LuaFunctionVoid caller{module["StartCommandLineSession"]};
    caller(mapName, isPerfTest);
}


// 0x008CE3D0
void func_InitializeSession() {
    if (Moho::CFG_GetArgOption("/debug", 0, nullptr)) {
        Moho::SCR_CreateDebugWindow();
        if (mSrcDebugWindow != nullptr) {
            lua_sethook(Moho::USER_GetLuaState()->m_state, func_DebugLuaHook, LUA_MASKLINE, 0);
        }
    }
    if (Moho::CFG_GetArgOption("/showlog", 0, nullptr)) {
        Moho::CON_Execute("WIN_ShowLogDialog true");
    }
    std::vector<std::string> args{};
    if (Moho::CFG_GetArgOption("/scenario", 1, &args) || Moho::CFG_GetArgOption("/map", 1, &args)) {
        Moho::OPTIONS_CreateInitialProfileIfNeeded("Default");
        std::string map = func_FindMapScenario(args[0].c_str());
        if (! func_StartCommandLineSession(map.c_str(), false)) {
            Moho::UI_StartSplashScreens();
            return;
        }
    } else if (Moho::CFG_GetArgOption("/replay", 1, &args)) {
        LuaPlus::LuaObject globals = Moho::USER_GetLuaState()->GetGlobals();
        globals["FrontEndData"].SetString("replay_filename", args[0].c_str());
        std::auto_ptr<Moho::SWldSessionInfo> sess = Moho::VCR_SetupReplaySession(args[0].c_str());
        if (sess.get() == nullptr) {
            Moho::WIN_OkBox("Ack!", gpg::STR_Printf("Unable to load game replay from %s", args[0].c_str()));
            Moho::UI_StartSplashScreens();
            return;
        }
        Moho::WLD_BeginSession(sess.get());
    } else {
        if (Moho::CFG_GetArgOption("/load", 1, &args)) {
            std::string map = args[0];
            if (! Moho::FILE_IsAbsolute(map.c_str())) {
                map = Moho::USER_GetSaveGameDir() + map + "." + Moho::USER_GetSaveGameExt();
            }
            Moho::CSavedGame game{map.c_str()};
            game.CreateSinglePlayerSessionInfo(??);
        }
        if (Moho::CFG_GetArgOption("/hostgame", 5, &args)) {
            Moho::OPTIONS_CreateInitialProfileIfNeeded(args[2].c_str());
            const char *mapName = func_FindMapScenario(args[4].c_str());
            const char *gameName = args[3].c_str();
            const char *playerName = args[2].c_str();
            int port = atoi(args[1].c_str());
            const char *protocol = args[0].c_str();
            func_StartHostLobbyUI(protocol, port, playerName, gameName, mapName);
        } else if (Moho::CFG_GetArgOption("/joingame", 3, &args)) {
            Moho::OPTIONS_CreateInitialProfileIfNeeded(args[2].c_str());
            const char *mapName = args[2].c_str();
            const char *gameName = args[1].c_str();
            const char *playerName = args[0].c_str();
            func_StartJoinLobbyUI(playerName, gameName, mapName);
        } else {
            if (Moho::CFG_GetArgOption("/perf", 0, nullptr)) {
                Moho::OPTIONS_CreateInitialProfileIfNeeded("Default");
                std::string test = func_FindMapScenario("PerfTest");
                if (func_StartCommandLineSession(test.c_str(), true)) {
                    Moho::UI_StartSplashScreens();
                }
                return;
            }
            if (! Moho::CFG_GetArgOption("/gpgnet", 1, &args)) {
                Moho::UI_StartSplashScreens();
                return;
            }
            u_long addr;
            u_short port;
            const char *info = args[0].c_str();
            if (Moho::NET_GetAddrInfo(info, 0, true, &addr, &port) && port != 0) {
                Moho::GPGNET_Attach(addr, port);
                LuaPlus::LuaState *state = Moho::USER_GetLuaState();
                if (UI_GetManager()->SetNewLuaState(state)) {
                    LuaPlus::LuaObject net = Moho::SCR_Import(state, "/lua/multiplayer/gpgnet.lua");
                    LuaPlus::LuaFunctionVoid caller{net["CreateUI"]};
                    caller();
                } else {
                    Moho::WIN_OkBox("Ack!", "UI_GetManager()->SetNewLuaState(state) failed!");
                    wxTheApp->ExitMainLoop();
                }
            } else {
                const char *msg = gpg::STR_Printf("Invalid address:port for connecting to the gpg.net client: \"%s\".", args[0].c_str());
                Moho::WIN_OkBox("Ack!", msg);
                wxTheApp->ExitMainLoop();
            }
        }
    }
}
