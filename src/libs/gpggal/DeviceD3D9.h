#include "gpggal/Device.h"

namespace gpg::gal {

// 0x00D4273C
class DeviceD3D9 : public gpg::gal::Device
{
public:
    int mCurThreadId;
    std::vector<gpg::gal::AdapterD3D9> mAdapters;
    gpg::gal::DeviceContext mDeviceContext;
    boost::shared_ptr<gpg::gal::PipelineStateD3D9> mPipelineState;
    IDirect3D9 *mDirect;
    IDirect3DDevice9 *mDevice;
    gpg::gal::Head *mHeads;
    int v32;
};

}
