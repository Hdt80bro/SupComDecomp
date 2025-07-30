#include <string>
#include <vector>

struct struct_AdapterMode
{
    int Width;
    int Height;
    int RefreshRate;
};

namespace gpg::gal {
  
// 0x00D423A4
class AdapterModeD3D9 : public struct_AdapterMode
{
public:

    virtual ~AdapterModeD3D9() = default; // 0x008E8E40
};

// 0x00D42808
class AdapterD3D9
{
public:
    int vendorId;
    int deviceId;
    std::string driver;
    std::string deviceName;
    std::string description;
    std::vector<gpg::gal::AdapterModeD3D9> modes;

    virtual ~AdapterD3D9() = default; // 0x008F0040
};


}
