#include <string>
//#include "wx/frame.h"

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
    virtual void Destroy() = 0;
    virtual bool HasFrame(); // 0x008CD470

    IWinApp(const char *title, const char *subtitle); // 0x008CD360
};

}
