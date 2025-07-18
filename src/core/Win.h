#include <Windows.h>
#include <string>
#include "wx/window.h"
#include "Task.h"

static WSupComFrame *mainWindow; // 0x010A63B8
static CScApp *supcomapp; // 0x010A63BC
static HHOOK windowHook; // 0x010A63C0
static gpg::time::Timer wakeupTimer; // 0x010A9B78
static float wakeupTimerDur; // 0x010A9B84
static Moho::CWaitHandleSet cwaithandleset; // 0x01103AE0
static Moho::FWaitHandleSet fwaithandleset; // 0x01103B30
static Moho::CTaskStage beforeEventsStage; // 0x011043CC
static Moho::CTaskStage beforeWaitStage; // 0x011043B4

namespace Moho {

class IWinApp;
class CWaitHandleSet;
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

bool CFG_GetArgOption(const char *flag, unsigned int a1, std::vector<std::string> *ebx0); // 0x0041B560
std::string CFG_GetArgs(); // 0x0041B690

}

int func_Dispatch(Moho::CTaskThread *); // 0x004091F0
void func_UserFrame(Moho::CTaskStage *); // 0x00409AC0
float func_ProbeWakeTimer(); // 0x004F1540
bool func_CorrectPlatformVersion(); // 0x004F2000
LRESULT func_WindowHook(int code, WPARAM wParam, _DWORD *lParam); // 0x004F2050

bool func_CheckMediaCenter(); // 0x008D4410
