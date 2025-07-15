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

void gpg::Stream::Write(const char *buf, int size) {
    if (size > this->LeftInWriteBuffer()) {
        this->VirtWrite(buf, size);
    } else {
        memcpy(this->writeStart, buf, size);
        this->writeStart += size;
    }
}
bool gpg::Stream::Close(gpg::Stream::Mode access) {
    this->VirtClose(access);
    return true;
}
int gpg::Stream::Read(char *buf, int size) {
    if (size > this->LeftInReadBuffer()) {
        size = this->VirtRead(buf, size);
    } else if (size) {
        memcpy(buf, this->readHead, size);
        this->readHead += size;
    }
    return size;
}