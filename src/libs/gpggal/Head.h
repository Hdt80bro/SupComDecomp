#include <string>
#include <vector>
#include <Windows.h>
#include "gpggal/Adapter.h"

namespace gpg::gal {

// 0x00D42128
class Head
{
public:
    HWND mHandle;
    HWND mWindow;
    bool mWindowed;
    int mWidth;
    int mWeight;
    int mFramesPerSecond;
    int mAntialiasingHigh;
    int mAntialiasingLow;
    std::string mName;
    std::vector<std::string> mStrs;
    std::vector<struct_AdapterMode> mAdapterModes;
    std::vector<int> mValidFormats2;
    std::vector<int> mValidFormats1;

    virtual ~Head() = default; // 0x00436990
};

}
