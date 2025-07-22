#include <string>
#include "gpgcore/Timer.h"

namespace Moho {

class IWinApp {
public:
    std::string title;
    std::string subtitle;
    int exitValue;

    virtual ~IWinApp() = default; // 0x008CD480
    virtual bool Func0(); // 0x008CD460
    virtual bool AppInitCommonServices(); // 0x004F1BA0
    virtual bool Main() = 0;
    virtual int OnNoMoreEvents() = 0;
    virtual void ScreenSaver() = 0;
    virtual bool HasFrame(); // 0x008CD470

    IWinApp(const char *title, const char *subtitle); // 0x008CD360
};

}

class CScApp : public Moho::IWinApp
{
public:
    int v16;
    int usingScreensaver;
    bool initialized;
    bool isMinimized;
    WSupComFrame *supcomFrame;
    WSupComFrame *frame;
    bool v21;
    gpg::time::Timer curTime;
    int v24;
    int v25;
    int v26;
    int v27;
    int v28;
    int v29;
    int v30;
    int v31;
    int v32;
    int v33;
    int v34;
    int v35;

    
    ~CScApp() override = default; // 0x008D1CB0
    bool Main() override; // 0x008CEDE0
    int OnNoMoreEvents() override; // 0x008D1470
    void ScreenSaver() override; // 0x008D0F20
    bool HasFrame() override; // 0x008CE1D0
};
