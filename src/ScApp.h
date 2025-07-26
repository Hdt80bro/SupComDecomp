//#include "wx/frame.h"
#include "gpgcore/Timer.h"
#include "core/App.h"

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

struct struct_RollingFrameRates
{
    float vals[10];
    int start;
    int end;

    void roll(float in) {
        if ((this->end + 1) % 10 == this->start) {
            this->start = (this->start + 1) % 10;
        }
        this->vals[this->start] = in;
        this->end = (this->end + 1) % 10;
    } // inline
    float median(); // 0x008D4B20
};


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
    struct_RollingFrameRates framerates;

    ~CScApp() override = default; // 0x008D1CB0
    bool Main() override; // 0x008CEDE0
    int OnNoMoreEvents() override; // 0x008D1470
    void Destroy() override; // 0x008D0F20
    bool HasFrame() override; // 0x008CE1D0
};
