#include "Stream.h"

// 0x00956E40
gpg::Stream::UnsupportedOperation::UnsupportedOperation()
    : std::logic_error{std::string{"Unsupported stream operation."}}
{}

// 0x00956F50
size_t gpg::Stream::VirtTell(gpg::Stream::Mode) {
    throw gpg::Stream::UnsupportedOperation{};
}

// 0x00956F90
size_t gpg::Stream::VirtSeek(gpg::Stream::Mode, gpg::Stream::SeekOrigin, __int64) {
    throw gpg::Stream::UnsupportedOperation{};
}

// 0x00956FB0
size_t gpg::Stream::VirtRead(char *, size_t) {
    throw gpg::Stream::UnsupportedOperation{};
}

// 0x00956DE0
size_t gpg::Stream::VirtReadNonBlocking(char *buf, size_t len) {
    return this->VirtRead(buf, len);
}

// 0x00956FD0
void gpg::Stream::VirtUnGetByte(int) {
    throw gpg::Stream::UnsupportedOperation{};
}

// 0x00956DF0
bool gpg::Stream::VirtAtEnd() {
    return false;
}

// 0x00956FF0
void gpg::Stream::VirtWrite(const char *data, size_t size) {
    throw gpg::Stream::UnsupportedOperation{};
}

// 0x00956E00
void gpg::Stream::VirtFlush() {}

// 0x00956E10
void gpg::Stream::VirtClose(gpg::Stream::Mode) {}

// 0x0043D130
void gpg::Stream::Write(const char *buf, size_t size) {
    if (size > this->LeftToWrite()) {
        this->VirtWrite(buf, size);
    } else {
        memcpy(this->mWritePtr, buf, size);
        this->mWritePtr += size;
    }
}

// 0x00955760
bool gpg::Stream::Close(gpg::Stream::Mode access) {
    this->VirtClose(access);
    return true;
}

// 0x0043D100
size_t gpg::Stream::Read(char *buf, size_t size) {
    if (size > this->LeftToRead()) {
        size = this->VirtRead(buf, size);
    } else if (size) {
        memcpy(buf, this->mReadPtr, size);
        this->mReadPtr += size;
    }
    return size;
}
