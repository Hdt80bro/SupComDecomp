
#include "core/WinApp.h"
#include "core/WinManaged.h"

#include "core/Task.h"
#include "gpgcore/String.h"
#include "gpgcore/Timer.h"
//#include "wx/frame.h"
//#include "wx/string.h"
//#include "wx/window.h"
#include <Windows.h>
#include <string>
#include <vector>


static wxFrame *sMainWindow; // 0x010A63B8
static Moho::IWinApp *sSupComApp; // 0x010A63BC
static HHOOK sWindowHook; // 0x010A63C0
static gpg::time::Timer sWakeupTimer; // 0x010A9B78
static float sWakeupTimerDur; // 0x010A9B84
static wxSplashScreen *sSplashScreenPtr; // 0x010A9BE4
static wxPNGHandler *sPngHandler; // 0x010C6D48
static Moho::CWaitHandleSet sCWaitHandleSet; // 0x01103AE0
static Moho::FWaitHandleSet sFWaitHandleSet; // 0x01103B30
static Moho::CTaskStage sBeforeEventsStage; // 0x011043CC
static Moho::CTaskStage sBeforeWaitStage; // 0x011043B4

namespace Moho {

class CWaitHandleSet;
class FWaitHandleSet;
class CTaskStage;

void WIN_ShowCrashDialog(const char *, const char *, _EXCEPTION_POINTERS *, int); // 0x004F1190
void WIN_CrashDialogDieHandler(const char *); // 0x004F1500
void WIN_AppExecute(Moho::IWinApp *); // 0x004F20B0
void WIN_AppRequestExit(); // 0x004F2400, 0x004F2B40
Moho::IWinApp *WIN_GetCurrentApp(); // 0x004F2410
Moho::CWaitHandleSet &WIN_GetWaitHandleSet(); // 0x004F2420
Moho::CTaskStage &WIN_GetBeforeEventsStage(); // 0x004F2480
Moho::CTaskStage &WIN_GetBeforeWaitStage(); // 0x004F24F0
void WIN_SetWakeupTimer(float); // 0x004F2560
wxWindow *WIN_GetMainWindow(); // 0x004F25B0
void WIN_SetMainWindow(wxWindow *); // 0x004F25C0
std::string WIN_GetClipboardText(); // 0x004F25D0
bool WIN_CopyToClipboard(const wchar_t *); // 0x004F2730
void WIN_OkBox(const char *, const char *); // 0x004F2800
bool WIN_YesNoBox(const char *, const char *); // 0x004F2900
std::string WIN_GetLastError(); // 0x004F2A00

void WINX_Exit(); // 0x004F3A60
wxString WINX_Printf(const char *args...); // 0x004F3B60
void WINX_InitSplash(gpg::StrArg); // 0x004F3CE0
void WINX_ExitSplash(); // 0x004F3F30
void WINX_PrecreateLogWindow(); // 0x004F3CD0 -> 0x004F67E0

bool CFG_GetArgOption(const char *opt, unsigned int arg, std::vector<std::string> *store); // 0x0041B560
std::string CFG_GetArgs(); // 0x0041B690

}

int func_Dispatch(Moho::CTaskThread *); // 0x004091F0
void func_UserFrame(Moho::CTaskStage *); // 0x00409AC0
float func_ProbeWakeTimer(); // 0x004F1540
bool func_HasCorrectPlatform(); // 0x004F2000
LRESULT func_WindowHook(int code, WPARAM wParam, DWORD *lParam); // 0x004F2050
