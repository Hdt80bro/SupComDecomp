#include "core/NetConnector.h"
#include "core/WinApp.h"
#include "gpgcore/Timer.h"
#include "gpggal/Device.h"
#include "wx/frame.h"

// 0x00E4F434
class WSupComFrame : public wxFrame
{
public:
    int v90;
    int v91;
    int v92;
    int v93;
    bool v94a;
    bool v94b;
    bool v94c;
    bool v94d;
};

// 0x00E4F6C0
class CScApp : public Moho::IWinApp
{
public:
    int v16;
    int mUsingScreensaver;
    bool mInitialized;
    bool mIsMinimized;
    WSupComFrame *mFram1;
    WSupComFrame *mFrame2;
    bool v21;
    gpg::time::Timer mCurTime;
    struct_RollingFloat<10> mFrameRates;

    ~CScApp() override = default; // 0x008D1CB0
    bool AppInit() override; // 0x008CEDE0
    int Main() override; // 0x008D1470
    void Destroy() override; // 0x008D0F20
    bool HasFrame() override; // 0x008CE1D0

    CScApp() = default;
    bool CreateAppFrame(std::string *title, BOOL maximized, wxPoint *pos, gpg::gal::DeviceContext *context); // 0x008CF8C0
    int CreateDevice(); // 0x008D0370
};

std::string func_FindMapScenario(const char *); // 0x008CE220
bool func_StartCommandLineSession(const char *mapName, bool isPerfTest); // 0x008CE2A0
void func_InitializeSession(); // 0x008CE3D0
bool func_StartJoinLobbyUI(const char *playerName, const char *gameName, const char *mapName); // 0x0083D030
bool func_StartHostLobbyUI(const char *protocol, int port, const char *playerName, const char *gameName, const char *mapName); // 0x0083CF20
