#include <string>
#include <vector>

struct struct_AdapterMode
{
    int mWidth;
    int mHeight;
    int mRefreshRate;
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
    int mVendorId;
    int mDeviceId;
    std::string mDriver;
    std::string mDeviceName;
    std::string mDescription;
    std::vector<gpg::gal::AdapterModeD3D9> mModes;

    virtual ~AdapterD3D9() = default; // 0x008F0040
};


}
