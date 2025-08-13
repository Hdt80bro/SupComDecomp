// known file

#include "Device.h"
#include "gpggal/Error.h"
#include <string>


gpg::gal::Device *sDeviceD3D; // 0x00F8E284

// 0x008E6720
bool gpg::gal::Device::IsReady() {
    return sDeviceD3D != nullptr;
}

// 0x008E6730
gpg::gal::Device *gpg::gal::Device::GetInstance() {
    return sDeviceD3D;
}

// 0x008E66E0
int gpg::gal::DeviceContext::GetHeadCount() {
    return this->mHeads.size();
}

// 0x008E69C0
gpg::gal::Head *gpg::gal::DeviceContext::GetHead(unsigned int idx) {
    if (this->mHeads.empty() || idx >= this->mHeads.size()) {
        GPGGAL_THROW("invalid head index"); //throw gpg::gal::Error{std::string{"c:\\work\\rts\\main\\code\\src\\libs\\gpggal\\Device.cpp"}, 91, std::string{"invalid head index"}};
    }
    return &this->mHeads[idx];
}

// 0x008E6940
void gpg::gal::Device::ClearTarget(gpg::gal::OutputContext &context) {
    this->mOutputContext = context;
}

// 0x008E6810
gpg::gal::OutputContext gpg::gal::Device::GetContext() {
    return this->mOutputContext;
}
