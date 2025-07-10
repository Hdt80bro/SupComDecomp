#include "Stream.h"

gpg::Stream::UnsupportedOperation::UnsupportedOperation()
    : std::logic_error{std::string{"Unsupported stream operation."}}
{}

size_t gpg::Stream::VirtTell(gpg::Stream::Mode) {
    throw gpg::Stream::UnsupportedOperation();
}
size_t gpg::Stream::VirtSeek(gpg::Stream::Mode, gpg::Stream::SeekOrigin, size_t) {
    throw gpg::Stream::UnsupportedOperation();
}
unsigned int gpg::Stream::VirtRead(char *, unsigned int) {
    throw gpg::Stream::UnsupportedOperation();
}
unsigned int gpg::Stream::VirtReadNonBlocking(char *buf, unsigned int len) {
    return this->VirtRead(buf, len);
}
void gpg::Stream::VirtUnGetByte(int) {
    throw gpg::Stream::UnsupportedOperation();
}
bool gpg::Stream::VirtAtEnd() {
    return false;
}
void gpg::Stream::VirtWrite(const char *data, unsigned int size) {
    throw gpg::Stream::UnsupportedOperation();
}
void gpg::Stream::VirtFlush() {}
void gpg::Stream::VirtClose(gpg::Stream::Mode) {}
