#include "gpggal/Device.h"

namespace gpg::gal {

// 0x00D4273C
class DeviceD3D9 : public gpg::gal::Device
{
public:
    int curThreadId;
    std::vector<gpg::gal::AdapterD3D9> adapters;
    gpg::gal::DeviceContext deviceContext;
    boost::shared_ptr<gpg::gal::PipelineStateD3D9> pipelineState;
    IDirect3D9 *idirect;
    IDirect3DDevice9 *device;
    gpg::gal::Head *heads;
    int v32;
};

}
