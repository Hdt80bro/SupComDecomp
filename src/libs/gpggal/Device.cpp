#include "Device.h"
#include <string>
#include "gpggal/Error.h"

// 0x008E69C0
gpg::gal::Head *gpg::gal::DeviceContext::GetHead(unsigned int idx) {
    if (this->heads.empty() || idx >= this->heads.size()) {
        GPGGAL_THROW("invalid head index"); //throw gpg::gal::Error{std::string{"c:\\work\\rts\\main\\code\\src\\libs\\gpggal\\Device.cpp"}, 91, std::string{"invalid head index"}};
    }
    return &this->heads[idx];
}

// 0x008E6940
void gpg::gal::Device::ClearTarget(gpg::gal::OutputContext &context) {
    this->outputContext = context;
}

// 0x008E6810
gpg::gal::OutputContext gpg::gal::Device::GetContext() {
    return this->outputContext;
}
