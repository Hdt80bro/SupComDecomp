#include <string>
#include <vector>
#include <Windows.h>
#include "gpggal/Adapter.h"

namespace gpg::gal {

// 0x00D42128
class Head
{
public:
    HWND handle;
    HWND window;
    bool windowed;
    int width;
    int height;
    int framesPerSecond;
    int antialiasingHigh;
    int antialiasingLow;
    std::string name;
    std::vector<std::string> strs;
    std::vector<struct_AdapterMode> adapterModes;
    std::vector<int> validFormats2;
    std::vector<int> validFormats1;

    virtual ~Head() = default; // 0x00436990
};

}
