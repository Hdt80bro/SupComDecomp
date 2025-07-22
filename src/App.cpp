#include "App.h"
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
    // ...
}

// 0x008CD470
bool Moho::IWinApp::HasFrame() {
    return false;
}

// 0x008CD360
Moho::IWinApp::IWinApp(const char *title, const char *subtitle) :
    title{title},
    subtitle{subtitle},
    exitValue{0}
{}


// 0x008CEDE0
bool CScApp::Main() {

}

// 0x008D1470
int CScApp::OnNoMoreEvents() {

}

// 0x008D0F20
void CScApp::ScreenSaver() {
    
}

// 0x008CE1D0
bool CScApp::HasFrame() {
    
}
