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
  gpg::gal::Head *Head; // edi
  int v6; // esi
  int height; // eax
  WSupComFrame *v8; // eax
  WSupComFrame *v10; // eax
  vtable_wxFrameBase *vtable; // edx
  int v12; // edx
  wxSize *v13; // ecx
  const char *Buf; // eax
  Moho::WRenViewport *v15; // ebx
  gpg::gal::DeviceContext *v16; // esi
  gpg::gal::Head *v17; // esi
  int v18; // eax
  WSupComFrame *v19; // eax
  const char *v20; // ecx
  WSupComFrame *v21; // eax
  Moho::CD3DDevice *Device; // esi
  bool validate; // cl
  gpg::gal::DeviceContext *v24; // eax
  Moho::WRenViewport *v25; // edi
  WSupComFrame *frame; // ecx
  Moho::CUIManager *v27; // ecx
  WSupComFrame *v28; // esi
  void *v30; // [esp-10h] [ebp-10Ch] BYREF
  wxPoint *v31; // [esp-Ch] [ebp-108h]
  wxSize *p_a4; // [esp-8h] [ebp-104h]
  int v33; // [esp-4h] [ebp-100h] BYREF
  int v34; // [esp+0h] [ebp-FCh] BYREF
  wxPaintEvent v35; // [esp+94h] [ebp-68h] BYREF
  wxSize v36; // [esp+B8h] [ebp-44h] BYREF
  std::vector_string v37; // [esp+C0h] [ebp-3Ch] BYREF
  int a5; // [esp+D0h] [ebp-2Ch]
  int a1; // [esp+D4h] [ebp-28h] BYREF
  WSupComFrame *v40; // [esp+D8h] [ebp-24h] BYREF
  wxSize a4; // [esp+DCh] [ebp-20h] BYREF
  WSupComFrame *v42; // [esp+E4h] [ebp-18h] BYREF
  Moho::WRenViewport *v43; // [esp+E4h] [ebp-18h] SPLIT
  int *v44; // [esp+ECh] [ebp-10h]
  int v45; // [esp+F8h] [ebp-4h]
  BOOL v46; // [esp+10Ch] [ebp+10h] SPLIT BYREF
  const char *v47; // [esp+10Ch] [ebp+10h] SPLIT

  v44 = &v34;
  memset(&v37.start, 0, 12);
  v45 = 0;
  Head = gpg::gal::DeviceContext::GetHead(context, 0);
  v6 = Head->windowed ? 0x200800 : 0x20400E40;
  a5 = v6;
  height = Head->height;
  a4.x = Head->width;
  a4.y = height;
  v8 = (WSupComFrame *)operator new(0x17Cu);
  v42 = v8;
  LOBYTE(v45) = 1;
  if ( v8 )
  {
    v33 = v6;
    p_a4 = &a4;
    v31 = pos;
    v30 = pos;
    v40 = (WSupComFrame *)&v30;
    if ( title->_Myres < 0x10 )
      v10 = WSupComFrame::WSupComFrame(v8, title->_Bx._Buf, v31, p_a4, v33);
    else
      v10 = WSupComFrame::WSupComFrame(v8, title->_Bx._Ptr, v31, p_a4, v33);
  }
  else
  {
    v10 = 0;
  }
  LOBYTE(v45) = 0;
  this->supcomFrame = v10;
  vtable = v10->vtable;
  if ( Head->windowed )
  {
    vtable->Show(v10, 0);
  }
  else
  {
    if ( maximized )
    {
      vtable->Func1(v10, 1);
    }
    else
    {
      vtable->DoSetClientSize((wxWindow *)v10, a4.x, a4.y);
      this->supcomFrame->vtable->DoGetClientSize((wxWindowBase *)this->supcomFrame, (int *)&v40, (int *)&v42);
      if ( v40 != (WSupComFrame *)a4.x || v42 != (WSupComFrame *)a4.y )
        gpg::Warnf("Unable to set requested size %i,%i. Results are undefined.", a4.x, a4.y);
    }
    v42 = (WSupComFrame *)&v33;
    if ( !Moho::CFG_GetArgOption("/splash", 0, 0) )
    {
      if ( sSplashScreenPtr )
        sSplashScreenPtr->vtable->dtr(sSplashScreenPtr, 1);
      sSplashScreenPtr = 0;
    }
    this->supcomFrame->vtable->Show(this->supcomFrame, 1);
  }
  if ( !Head->windowed && maximized )
  {
    this->supcomFrame->vtable->DoGetClientSize((wxWindowBase *)this->supcomFrame, &v46, (int *)&v42);
    a4.x = v46;
    a4.y = (int)v42;
    Head->width = v46;
    Head->height = a4.y;
  }
  sMainWindow = this->supcomFrame;
  sMainWindow->vtable->DoGetClientSize((wxWindowBase *)sMainWindow, &v46, (int *)&v42);
  a1 = v46;
  v40 = v42;
  LOBYTE(v12) = (unsigned int)gpg::gal::DeviceContext::GetHeadCount(context) > 1;
  v33 = v12;
  p_a4 = v13;
  Buf = title->_Bx._Buf;
  v47 = title->_Bx._Buf;
  if ( title->_Myres >= 0x10 )
    Buf = *(const char **)Buf;
  v15 = Moho::REN_CreateGameViewport(&a1, sMainWindow, Buf, v33);
  v43 = v15;
  Head->handle = v15->m_parent->Moho::WD3DViewport::wxWindow_base::wxWindowBase_base::vtable->GetHandle(v15->m_parent);
  Head->window = v15->vtable->GetHandle(v15);
  v16 = context;
  if ( (unsigned int)gpg::gal::DeviceContext::GetHeadCount(context) > 1 )
  {
    v17 = gpg::gal::DeviceContext::GetHead(v16, 1u);
    v18 = v17->height;
    v36.x = v17->width;
    v36.y = v18;
    v19 = (WSupComFrame *)operator new(0x17Cu);
    v40 = v19;
    LOBYTE(v45) = 2;
    if ( v19 )
    {
      v33 = a5;
      p_a4 = &v36;
      v31 = pos;
      v30 = &v36;
      pos = (wxPoint *)&v30;
      v20 = v47;
      if ( title->_Myres >= 0x10 )
        v20 = *(const char **)v47;
      v21 = WSupComFrame::WSupComFrame(v19, v20, v31, p_a4, v33);
    }
    else
    {
      v21 = 0;
    }
    LOBYTE(v45) = 0;
    this->frame = v21;
    ((void (__stdcall *)(bool))v21->vtable->Show)(!v17->windowed);
    v17->handle = Head->handle;
    v17->window = this->frame->vtable->GetHandle(this->frame);
  }
  wxEvent::wxEvent(&v35, 0, wxEVT_PAINT);
  v35.vtable = (vtable_wxObject *)&wxPaintEvent::`vftable';
  LOBYTE(v45) = 4;
  Moho::WD3DViewport::OnPaint(v15, (int)&v35);
  LOBYTE(v45) = 0;
  v35.vtable = &wxObject::`vftable';
  wxEvent::UnRef(&v35);
  LOBYTE(v45) = 7;
  Device = Moho::D3D_GetDevice();
  Device->vtable->Clear2(Device, 1);
  v47 = (const char *)&v33;
  validate = !Moho::CFG_GetArgOption("/novalidate", 0, 0);
  v24 = context;
  context->v16 = validate;
  func_CreateDeviceD3D(v24);
  v25 = v43;
  Device->vtable->SetRenViewport(Device, v43);
  Device->vtable->Clear2(Device, 0);
  v45 = 0;
  if ( sSplashScreenPtr )
    sSplashScreenPtr->vtable->dtr(sSplashScreenPtr, 1);
  sSplashScreenPtr = 0;
  this->supcomFrame->vtable->Show(this->supcomFrame, 1);
  frame = this->frame;
  if ( frame )
    frame->vtable->Show(frame, 1);
  sRenViewport = v25;
  v25->vtable->SetFocus((wxWindow *)v25);
  Moho::WINX_PrecreateLogWindow();
  v27 = sUIManager;
  if ( sUIManager )
  {
    sUIManager->vtable->Func1(sUIManager, v25, this->supcomFrame);
    v27 = sUIManager;
  }
  v28 = this->frame;
  if ( v28 && v27 )
    v27->vtable->Func1(v27, (Moho::WRenViewport *)v28, v28);
  return 1;
}

// 0x008D0370
int CScApp::CreateDevice() {
  bool ArgOption; // al
  gpg::gal::Head *v3; // eax
  gpg::gal::Head *Head; // eax
  gpg::gal::Head *v5; // eax
  gpg::gal::Head *v6; // edi
  gpg::gal::Head *v7; // eax
  std::string *v8; // esi
  gpg::gal::Head *v9; // ebx
  const char *Buf; // eax
  int v11; // eax
  int v12; // ecx
  const char *Ptr; // eax
  int v14; // eax
  int v15; // ecx
  const char *v16; // eax
  int v17; // eax
  int v18; // ecx
  const char *v19; // eax
  int v20; // eax
  int v21; // ecx
  gpg::gal::Head *v22; // eax
  gpg::gal::Head *v23; // edi
  const char *v24; // eax
  int v25; // eax
  int v26; // ecx
  const char *v27; // eax
  int v28; // eax
  int v29; // ecx
  gpg::gal::Head *v30; // eax
  gpg::gal::Head *v31; // edi
  const char *v32; // eax
  int v33; // eax
  int v34; // ecx
  const char *v35; // eax
  int v36; // eax
  int v37; // ecx
  std::string *String; // eax
  std::string *v39; // eax
  bool v40; // al
  unsigned int v41; // ebx
  gpg::gal::Head *v42; // eax
  gpg::gal::Head *v43; // esi
  gpg::gal::Head *v44; // ebp
  std::string *v45; // eax
  int v46; // ecx
  int height; // eax
  std::string *v48; // eax
  int z1; // edx
  std::string *v50; // eax
  bool v51; // bl
  std::string *v52; // eax
  gpg::gal::Head *v53; // eax
  int v54; // ecx
  int v55; // edx
  gpg::gal::Head *v56; // esi
  bool OptionAmong; // al
  const char *v58; // eax
  char v59; // bl
  bool (__thiscall *GetBoolean)(Moho::CUserPrefs *, std::string *, bool); // edx
  bool v61; // al
  std::string::_Bxty *p_Bx; // eax
  LONG v63; // eax
  LONG v64; // eax
  HMONITOR v65; // eax
  LONG x; // eax
  int y; // eax
  int Int; // ebp
  gpg::gal::Head *v69; // eax
  int v70; // edi
  int v71; // ebp
  gpg::gal::Head *v72; // eax
  bool v73; // al
  gpg::gal::DeviceContext *v74; // eax
  bool v75; // al
  int result; // eax
  gpg::gal::DeviceD3D9 *Instance; // eax
  size_t Mysize; // [esp+0h] [ebp-19Ch]
  const char *v79[6]; // [esp+8h] [ebp-194h] BYREF
  char v80; // [esp+22h] [ebp-17Ah]
  char v81; // [esp+23h] [ebp-179h]
  unsigned int v82; // [esp+24h] [ebp-178h]
  const char *v83; // [esp+28h] [ebp-174h]
  POINT pt; // [esp+2Ch] [ebp-170h] BYREF
  const char **v85; // [esp+34h] [ebp-168h]
  Moho::CUserPrefs *maximized; // [esp+38h] [ebp-164h] MAPDST
  std::string *a4[4]; // [esp+3Ch] [ebp-160h] BYREF
  __int128 v88; // [esp+4Ch] [ebp-150h] BYREF
  Resolution v89; // [esp+4Ch] [ebp-150h] SPLIT BYREF
  unsigned int v90; // [esp+64h] [ebp-138h]
  gpg::gal::DeviceContext v91; // [esp+68h] [ebp-134h] BYREF
  gpg::Rect2i a1; // [esp+9Ch] [ebp-100h] BYREF
  Resolution v93; // [esp+9Ch] [ebp-100h] SPLIT BYREF
  Resolution v94; // [esp+9Ch] [ebp-100h] SPLIT BYREF
  std::string arg4; // [esp+B8h] [ebp-E4h] BYREF
  gpg::gal::DeviceContext mi; // [esp+D4h] [ebp-C8h] BYREF
  gpg::gal::Head a2; // [esp+108h] [ebp-94h] BYREF
  int v98; // [esp+198h] [ebp-4h]

  v82 = 0;
  sDeviceLock = 1;
  arg4._Myres = 15;
  arg4._Mysize = 0;
  arg4._Bx._Buf[0] = 0;
  std::string::string(&arg4, "Forged Alliance", 0xFu);
  v98 = 1;
  memset(&a4[1], 0, 12);
  maximized = Moho::USER_GetPreferences();
  v85 = v79;
  Moho::d3d_WindowsCursor = Moho::CFG_GetArgOption("/D3D10", 0, 0);
  v85 = v79;
  ArgOption = Moho::CFG_GetArgOption("/D3D10", 0, 0);
  gpg::gal::DeviceContext::DeviceContext(&v91, ArgOption + 1);
  LOBYTE(v98) = 2;
  v85 = v79;
  v91.vsync = Moho::OPTIONS_GetInt("vsync") == 1;
  v3 = gpg::gal::Head::Head(&a2);
  LOBYTE(v98) = 3;
  gpg::gal::DeviceContext::AddHead(&v91, v3);
  LOBYTE(v98) = 2;
  gpg::gal::Head::~Head(&a2);
  Head = gpg::gal::DeviceContext::GetHead(&v91, 0);
  Head->windowed = 1;
  Head->width = Moho::wnd_DefaultCreateWidth;
  Head->height = Moho::wnd_DefaultCreateHeight;
  Head->framesPerSecond = 60;
  pt = (POINT)wxDefaultPosition;
  v80 = 0;
  LOBYTE(v85) = 0;
  if ( func_ValidWindowOption(a4) )
  {
    v5 = gpg::gal::Head::Head(&a2);
    LOBYTE(v98) = 4;
    gpg::gal::DeviceContext::AddHead(&v91, v5);
    LOBYTE(v98) = 2;
    gpg::gal::Head::~Head(&a2);
    v6 = gpg::gal::DeviceContext::GetHead(&v91, 0);
    v7 = gpg::gal::DeviceContext::GetHead(&v91, 1u);
    v8 = a4[1];
    v9 = v7;
    v6->windowed = 1;
    v7->windowed = 1;
    if ( v8->_Myres < 0x10 )
      Buf = v8->_Bx._Buf;
    else
      Buf = v8->_Bx._Ptr;
    v11 = atoi(Buf);
    v12 = Moho::wnd_DefaultCreateWidth;
    if ( Moho::wnd_DefaultCreateWidth < v11 )
      v12 = v11;
    v6->width = v12;
    if ( v8[1]._Myres < 0x10 )
      Ptr = v8[1]._Bx._Buf;
    else
      Ptr = v8[1]._Bx._Ptr;
    v14 = atoi(Ptr);
    v15 = Moho::wnd_DefaultCreateHeight;
    if ( Moho::wnd_DefaultCreateHeight < v14 )
      v15 = v14;
    v6->height = v15;
    if ( a4[2] - v8 == 4 )
    {
      if ( v8[2]._Myres < 0x10 )
        v16 = v8[2]._Bx._Buf;
      else
        v16 = v8[2]._Bx._Ptr;
      v17 = atoi(v16);
      v18 = Moho::wnd_DefaultCreateWidth;
      if ( Moho::wnd_DefaultCreateWidth < v17 )
        v18 = v17;
      v9->width = v18;
      if ( v8[3]._Myres < 0x10 )
        v19 = v8[3]._Bx._Buf;
      else
        v19 = v8[3]._Bx._Ptr;
      v20 = atoi(v19);
      v21 = Moho::wnd_DefaultCreateHeight;
      if ( Moho::wnd_DefaultCreateHeight < v20 )
        v21 = v20;
      v9->height = v21;
      sAdapterNotCLOverridden = 0;
    }
    else
    {
      v9->width = v6->width;
      v9->height = v6->height;
      sAdapterNotCLOverridden = 0;
    }
  }
  else
  {
    if ( !func_ValidFullscreenArg(a4) )
    {
      if ( func_ValidWindowedOption(a4) )
      {
        v30 = gpg::gal::DeviceContext::GetHead(&v91, 0);
        v8 = a4[1];
        v31 = v30;
        v30->windowed = 0;
        if ( v8->_Myres < 0x10 )
          v32 = v8->_Bx._Buf;
        else
          v32 = v8->_Bx._Ptr;
        v33 = atoi(v32);
        v34 = Moho::wnd_MinCmdLineWidth;
        if ( Moho::wnd_MinCmdLineWidth < v33 )
          v34 = v33;
        v31->width = v34;
        if ( v8[1]._Myres < 0x10 )
          v35 = v8[1]._Bx._Buf;
        else
          v35 = v8[1]._Bx._Ptr;
        v36 = atoi(v35);
        v37 = Moho::wnd_MinCmdLineHeight;
        if ( Moho::wnd_MinCmdLineHeight < v36 )
          v37 = v36;
        v31->height = v37;
        v80 = 1;
        sAdapterNotCLOverridden = 0;
      }
      else
      {
        v82 = (unsigned int)v79;
        String = Moho::OPTIONS_GetString((std::string *)&a1, "secondary_adapter");
        v79[0] = (const char *)8;
        LOBYTE(v98) = 5;
        Mysize = String->_Mysize;
        v82 = 1;
        if ( !std::operator<<char,std::char_traits<char>,std::allocator<char>>(String, 0, Mysize, "disabled", 8u)
          || (v83 = (const char *)v79,
              v39 = Moho::OPTIONS_GetString((std::string *)&v88, "primary_adapter"),
              v98 = 6,
              v82 = 3,
              v40 = func_StrCmp(v39, "windowed"),
              v81 = 1,
              !v40) )
        {
          v81 = 0;
        }
        v41 = v82;
        if ( (v82 & 2) != 0 )
        {
          v41 = v82 & 0xFFFFFFFD;
          v82 &= ~2u;
          std::string::~string(&v88);
        }
        v98 = 2;
        if ( (v41 & 1) != 0 )
        {
          v82 = v41 & 0xFFFFFFFE;
          std::string::~string(&a1);
        }
        if ( v81 )
        {
          v42 = gpg::gal::Head::Head(&a2);
          LOBYTE(v98) = 7;
          gpg::gal::DeviceContext::AddHead(&v91, v42);
          LOBYTE(v98) = 2;
          gpg::gal::Head::~Head(&a2);
          v43 = gpg::gal::DeviceContext::GetHead(&v91, 0);
          v44 = gpg::gal::DeviceContext::GetHead(&v91, 1u);
          v43->windowed = 1;
          v44->windowed = 1;
          v83 = (const char *)v79;
          v45 = Moho::OPTIONS_GetString((std::string *)&v88, "primary_adapter");
          LOBYTE(v98) = 8;
          v93.vtable = &Resolution::`vftable';
          func_StrListToRect(&v93, v45);
          LOBYTE(v98) = 10;
          std::string::~string(&v88);
          v46 = v93.v2;
          height = v93.height;
          v43->width = v93.width;
          v43->height = height;
          v43->framesPerSecond = v46;
          v83 = (const char *)v79;
          v48 = Moho::OPTIONS_GetString((std::string *)&mi, "secondary_adapter");
          LOBYTE(v98) = 11;
          func_StrListToRect(&v89, v48);
          std::string::~string(&mi);
          z1 = v89.v2;
          *(_QWORD *)&v44->width = *(_QWORD *)&v89.width;
          v44->framesPerSecond = z1;
          LOBYTE(v98) = 2;
          v8 = a4[1];
          goto LABEL_62;
        }
        v83 = (const char *)v79;
        v50 = Moho::OPTIONS_GetString((std::string *)&mi, "primary_adapter");
        LOBYTE(v98) = 12;
        v51 = func_StrCmp(v50, "windowed");
        LOBYTE(v98) = 2;
        std::string::~string(&mi);
        if ( v51 )
        {
          v83 = (const char *)v79;
          v52 = Moho::OPTIONS_GetString((std::string *)&mi, "primary_adapter");
          LOBYTE(v98) = 13;
          v94.vtable = &Resolution::`vftable';
          func_StrListToRect(&v94, v52);
          LOBYTE(v98) = 15;
          std::string::~string(&mi);
          v53 = gpg::gal::DeviceContext::GetHead(&v91, 0);
          v54 = v94.height;
          v53->width = v94.width;
          v55 = v94.v2;
          LOBYTE(v98) = 2;
          v8 = a4[1];
          v53->windowed = 1;
          v53->height = v54;
          v53->framesPerSecond = v55;
          goto LABEL_62;
        }
        v56 = gpg::gal::DeviceContext::GetHead(&v91, 0);
        v79[0] = "Windows.Main.width";
        v56->windowed = 0;
        std::string::string((std::string *)&v88, v79[0]);
        LOBYTE(v98) = 16;
        v56->width = maximized->vtable->GetInteger(maximized, (std::string *)&v88, Moho::wnd_DefaultCreateWidth);
        LOBYTE(v98) = 2;
        std::string::~string(&v88);
        std::string::string((std::string *)&v88, "Windows.Main.height");
        LOBYTE(v98) = 17;
        v56->height = maximized->vtable->GetInteger(maximized, (std::string *)&v88, Moho::wnd_DefaultCreateHeight);
        LOBYTE(v98) = 2;
        std::string::~string(&v88);
        v8 = a4[1];
      }
      LOBYTE(v85) = 1;
      goto LABEL_62;
    }
    v22 = gpg::gal::DeviceContext::GetHead(&v91, 0);
    v8 = a4[1];
    v23 = v22;
    v22->windowed = 1;
    if ( v8->_Myres < 0x10 )
      v24 = v8->_Bx._Buf;
    else
      v24 = v8->_Bx._Ptr;
    v25 = atoi(v24);
    v26 = Moho::wnd_DefaultCreateWidth;
    if ( Moho::wnd_DefaultCreateWidth < v25 )
      v26 = v25;
    v23->width = v26;
    if ( v8[1]._Myres < 0x10 )
      v27 = v8[1]._Bx._Buf;
    else
      v27 = v8[1]._Bx._Ptr;
    v28 = atoi(v27);
    v29 = Moho::wnd_DefaultCreateHeight;
    if ( Moho::wnd_DefaultCreateHeight < v28 )
      v29 = v28;
    v23->height = v29;
    sAdapterNotCLOverridden = 0;
  }
LABEL_62:
  if ( gpg::gal::DeviceContext::GetHeadCount(&v91) == 1 )
  {
    OptionAmong = func_FindOptionAmong(0, &str_adapter, 1, a4);
    v8 = a4[1];
    if ( OptionAmong )
    {
      if ( a4[1]->_Myres < 0x10 )
        v58 = a4[1]->_Bx._Buf;
      else
        v58 = a4[1]->_Bx._Ptr;
      v91.adapter = atoi(v58);
    }
  }
  v59 = func_CLHasMaximize();
  sAdapterNotCLOverridden &= (v59 != 0) - 1;
  LOBYTE(maximized) = !v80
                   && !gpg::gal::DeviceContext::GetHead(&v91, 0)->windowed
                   && (v59
                    || (std::string::string((std::string *)&v89, "Windows.Main.maximized"),
                        LOBYTE(v98) = 18,
                        GetBoolean = maximized->vtable->GetBoolean,
                        v82 |= 4u,
                        GetBoolean(maximized, (std::string *)&v89, 0)));
  v98 = 2;
  if ( (v82 & 4) != 0 && v90 >= 0x10 )
    operator delete((void *)v89.width);
  if ( !gpg::gal::DeviceContext::GetHead(&v91, 0)->windowed )
  {
    v83 = (const char *)v79;
    v61 = Moho::CFG_GetArgOption("/position", 2, (std::vector_string *)a4);
    v8 = a4[1];
    if ( v61 )
    {
      if ( a4[1]->_Myres < 0x10 )
        p_Bx = &a4[1]->_Bx;
      else
        p_Bx = (std::string::_Bxty *)a4[1]->_Bx._Ptr;
      pt.x = atoi(p_Bx->_Buf);
      if ( v8[1]._Myres < 0x10 )
        pt.y = atoi(v8[1]._Bx._Buf);
      else
        pt.y = atoi(v8[1]._Bx._Ptr);
    }
    else
    {
      std::string::string((std::string *)&v89, "Windows.Main.x");
      LOBYTE(v98) = 19;
      v63 = maximized->vtable->GetInteger(maximized, (std::string *)&v89, pt.x);
      LOBYTE(v98) = 2;
      pt.x = v63;
      if ( v90 >= 0x10 )
        operator delete((void *)v89.width);
      std::string::string((std::string *)&v89, "Windows.Main.y");
      LOBYTE(v98) = 20;
      v64 = maximized->vtable->GetInteger(maximized, (std::string *)&v89, pt.y);
      LOBYTE(v98) = 2;
      pt.y = v64;
      if ( v90 >= 0x10 )
        operator delete((void *)v89.width);
    }
    v65 = MonitorFromPoint(pt, 2u);
    mi.vtable = 40;
    GetMonitorInfoW(v65, (LPMONITORINFO)&mi);
    x = pt.x;
    if ( pt.x >= (int)mi.adapter )
      x = mi.adapter;
    if ( x < mi.deviceType )
      x = mi.deviceType;
    pt.x = x;
    y = pt.y;
    if ( pt.y >= *(int *)&mi.vsync )
      y = *(_DWORD *)&mi.vsync;
    if ( y < *(int *)&mi.v16 )
      y = *(_DWORD *)&mi.v16;
    pt.y = y;
  }
  v83 = (const char *)v79;
  Int = Moho::OPTIONS_GetInt("antialiasing");
  v69 = gpg::gal::DeviceContext::GetHead(&v91, 0);
  v70 = Int >> 5;
  v71 = Int & 0x1F;
  v69->antialiasingHigh = v70;
  v69->antialiasingLow = v71;
  if ( (unsigned int)gpg::gal::DeviceContext::GetHeadCount(&v91) > 1 )
  {
    v72 = gpg::gal::DeviceContext::GetHead(&v91, 1u);
    v72->antialiasingHigh = v70;
    v72->antialiasingLow = v71;
  }
  if ( !CScApp::CreateAppFrame(app, &arg4, (BOOL)maximized, (wxPoint *)&pt, &v91) )
  {
    v83 = (const char *)v79;
    v73 = Moho::CFG_GetArgOption("/D3D10", 0, 0);
    v74 = gpg::gal::DeviceContext::DeviceContext(&mi, v73 + 1);
    LOBYTE(v98) = 21;
    func_CpyDeviceContext(&v91, v74);
    LOBYTE(v98) = 2;
    gpg::gal::DeviceContext::~DeviceContext(&mi);
    gpg::gal::Head::Head(&a2);
    LOBYTE(v98) = 22;
    a2.height = Moho::wnd_DefaultCreateHeight;
    a2.windowed = 1;
    a2.width = Moho::wnd_DefaultCreateWidth;
    gpg::gal::DeviceContext::AddHead(&v91, &a2);
    v75 = CScApp::CreateAppFrame(app, &arg4, 0, (wxPoint *)&pt, &v91);
    LOBYTE(v98) = 2;
    if ( !v75 )
    {
      gpg::gal::Head::~Head(&a2);
      LOBYTE(v98) = 1;
      gpg::gal::DeviceContext::~DeviceContext(&v91);
      std::vector_string::~vector_string((std::vector_string *)a4);
      if ( arg4._Myres >= 0x10 )
        operator delete((void *)arg4._Bx._Ptr);
      LOBYTE(result) = 0;
      return result;
    }
    gpg::gal::Head::~Head(&a2);
  }
  Instance = gpg::gal::Device::GetInstance();
  Instance->vtable->GetDeviceContext(Instance);
  func_SetupPrimaryAdapterSettings();
  func_SetupSecondaryAdapterSettings((char)v85);
  func_CreateFidelityPresets();                 // patched out
  func_SetupFidelitySettings();
  func_SetupShadowQualitySettings();
  func_SetupAntiAliasingSettings();
  func_SetupBasicMovieManager();
  sDeviceLock = 0;
  LOBYTE(v98) = 1;
  gpg::gal::DeviceContext::~DeviceContext(&v91);
  if ( v8 )
  {
    v79[0] = v83;
    func_DestroyStringsRange(v8, a4[2]);
    operator delete(v8);
  }
  if ( arg4._Myres >= 0x10 )
    operator delete((void *)arg4._Bx._Ptr);
  LOBYTE(result) = 1;
  return result;
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
