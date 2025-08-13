//#include "wx/frame.h"
#include <string>

namespace Moho {

// 0x00E4F408
class IWinApp {
public:
    std::string mTitle;
    std::string mSubtitle;
    int mExitValue;

    virtual ~IWinApp() = default; // 0x008CD480
    virtual bool Func0(); // 0x008CD460
    virtual bool AppInitCommonServices(); // 0x004F1BA0
    virtual bool AppInit() = 0;
    virtual int Main() = 0;
    virtual void Destroy() = 0;
    virtual bool HasFrame(); // 0x008CD470

    IWinApp(const char *title, const char *subtitle); // 0x008CD360
};

}
