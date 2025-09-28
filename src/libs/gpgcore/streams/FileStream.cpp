// known file

#include "FileStream.h"
#include "gpgcore/General.h"
#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"
#include <Windows.h>

int sCreationDisposition[] = { // 0x00D494B0
    CREATE_NEW,
    OPEN_ALWAYS,
    OPEN_EXISTING,
    TRUNCATE_EXISTING,
    0,
    0,
    0,
    0,
};
int sMoveMethod[] = { // 0x00D4953C
    FILE_BEGIN,
    FILE_CURRENT,
    FILE_END,
};

// 0x00955890
gpg::FileStream::IOError::IOError(int id) :
    std::runtime_error{gpg::GetWin32ErrorString(id)},
    mId{id}
{}

// 0x00955870
gpg::FileStream::~FileStream() {
    this->Close(this->mAccessKind);
}

// 0x00955CE0
size_t gpg::FileStream::VirtTell(gpg::Stream::Mode mode) {
    if ((this->mAccessKind & mode) == 0) {
        throw std::invalid_argument{std::string{"Invalid mode for Tell()"}};
    }
    LARGE_INTEGER size;
    size.QuadPart = 0;
    LARGE_INTEGER newFilePointer;
    if (! ::SetFilePointerEx(this->mHandle, size, &newFilePointer, FILE_CURRENT)) {
        throw gpg::FileStream::IOError{::GetLastError()};
    }
    size_t pos = newFilePointer.LowPart;
    if (! this->DoneReading()) {
        pos -= this->LeftToRead();
    }
    if (this->SomethingWritten()) {
        pos += this->BytesWritten();
    }
    return pos;
}

// 0x00955DF0
size_t gpg::FileStream::VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, __int64 pos) {
    if ((this->mAccessKind & mode) == 0) {
        throw std::invalid_argument{std::string{"Invalid mode for Seek()"}};
    }
    if (orig > gpg::Stream::OriginEnd) {
        throw std::invalid_argument{std::string{"Invalid origin for Seek()"}};
    }
    if (this->SomethingWritten()) {
        this->DoWrite(this->mWriteStart, this->BytesWritten());
    }
    if (this->CanReading() && orig == gpg::Stream::OriginCurr) {
        pos -= this->LeftToRead();
    }
    LARGE_INTEGER dist;
    dist.QuadPart = pos;
    LARGE_INTEGER newFilePointer;
    if (! ::SetFilePointerEx(this->mHandle, dist, &newFilePointer, sMoveMethod[orig])) {
        throw gpg::FileStream::IOError{::GetLastError()};
    }
    this->mReadEnd = nullptr;
    this->mReadPtr = nullptr;
    this->mReadStart = nullptr;
    this->mWriteEnd = nullptr;
    this->mWritePtr = nullptr;
    this->mWriteStart = nullptr;
    return newFilePointer.QuadPart;
}

// 0x00956180
void gpg::FileStream::VirtWrite(const char *str, size_t len) {
    if ((this->mAccessKind & gpg::Stream::ModeWrite) == 0) {
        throw std::runtime_error{std::string{"Attempt to write to a file that isn't open for output."}};
    }
    int size = this->LeftToWrite();
    if (len > size) {
        if (size != 0 && len - size < this->mBuff.Size()) {
            memcpy(this->mWritePtr, str, size);
            this->mWritePtr += size;
            str += size;
            len -= size;
        }
        this->DoWrite(this->mWriteStart, this->BytesWritten());
        if (len >= this->mBuff.Size()) {
            this->DoWrite(str, len);
            return;
        }
    }
    memcpy(this->mWritePtr, str, len);
    this->mWritePtr += len;
}

// 0x00955F80
size_t gpg::FileStream::VirtRead(char *buff, size_t len) {
    if ((this->mAccessKind & gpg::Stream::ModeRead) == 0) {
        throw std::runtime_error{std::string{"Attempt to read from a file that isn't open for input."}};
    }
    size_t readBuff = this->LeftToRead();
    int readAmt = 0;
    while (readBuff < len) {
        if (readBuff != 0) {
            memcpy(&buff[readAmt], this->mReadPtr, readBuff);
            this->mReadPtr += readBuff;
            readAmt += readBuff;
            buff += readBuff;
            len -= readBuff;
        }
        if (len >= this->mBuff.Size()) {
            size_t amt = this->DoRead(buff, len);
            if (amt != 0) {
                readAmt += amt;
                buff += amt;
                len -= amt;
            } else {
                return readAmt;
            }
        } else {
            size_t amt = this->DoRead(this->mBuff.begin(), this->mBuff.Size());
            if (amt == 0) {
                return readAmt;
            }
            this->mReadPtr = this->mBuff.begin();
            this->mReadStart = this->mBuff.begin();
            this->mReadEnd = &this->mBuff[amt];
        }
        readBuff = this->mReadEnd - this->mReadPtr;
    }
    memcpy(buff, this->mReadPtr, len);
    this->mReadPtr += len;
    readAmt += len;
    return readAmt;
}

// 0x009560C0
bool gpg::FileStream::VirtAtEnd() {
    if ((this->mAccessKind & gpg::Stream::ModeRead) == 0) {
        throw std::runtime_error{std::string{"Attempt to read from a file that isn't open for input."}};
    }
    if (this->CanRead()) {
        return false;
    }
    size_t amt = this->DoRead(this->mBuff.begin(), this->mBuff.Size());
    this->mReadPtr = this->mBuff.begin();
    this->mReadStart = this->mBuff.begin();
    this->mReadEnd = &this->mBuff[amt];
    return amt == 0;
}

// 0x00956290
void gpg::FileStream::VirtFlush() {
    if ((this->mAccessKind & gpg::Stream::ModeWrite) == 0) {
        throw std::runtime_error{std::string{"Attempt to write to a file that isn't open for output."}};
    }
    if (this->SomethingWritten()) {
        this->DoWrite(this->mWriteStart, this->BytesWritten());
    }
}

// 0x00956320
void gpg::FileStream::VirtClose(gpg::Stream::Mode mode) {
    if ((this->mAccessKind & mode & gpg::Stream::ModeWrite) != 0) {
        if (this->SomethingWritten()) {
            this->DoWrite(this->mWriteStart, this->BytesWritten());
        }
        this->mAccessKind &=~ gpg::Stream::ModeWrite;
        this->mWriteEnd = nullptr;
        this->mWritePtr = nullptr;
        this->mWriteStart = nullptr;
    }
    if ((this->mAccessKind & mode & gpg::Stream::ModeRead) != 0) {
        this->mAccessKind &=~ gpg::Stream::ModeRead;
        this->mReadEnd = nullptr;
        this->mReadPtr = nullptr;
        this->mReadStart = nullptr;
    }
    if (this->mHandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(this->mHandle);
        this->mHandle = INVALID_HANDLE_VALUE;
        this->mBuff.Reset();
    }
}


// 0x00955BD0
gpg::FileStream::FileStream(const char *filename, gpg::Stream::Mode accessKind, gpg::EFileAttributes attributes, size_t buffSize) :
    gpg::Stream{},
    mHandle{0},
    mAccessKind{gpg::Stream::ModeNone}
{
    this->mBuff = gpg::AllocMemBuffer(buffSize);
    this->DoOpen(filename, accessKind, attributes);
}

// 0x00955990
void gpg::FileStream::DoOpen(const char *filename, gpg::Stream::Mode accessKind, gpg::EFileAttributes attributes) {
    std::wstring fileStr = gpg::STR_Utf8ToWide(filename);
    unsigned int access = 0;
    if ((accessKind & gpg::Stream::ModeRead) != 0) {
        access |= GENERIC_READ;
    }
    if ((accessKind & gpg::Stream::ModeWrite) != 0) {
        access |= GENERIC_WRITE;
    }
    unsigned int share = 0;
    if ((attributes & gpg::EFileAttributes::FA_ShareRead) != 0) {
        share |= FILE_SHARE_READ;
    }
    if ((attributes & gpg::EFileAttributes::FA_ShareWrite) != 0) {
        share |= FILE_SHARE_WRITE;
    }
    unsigned int flags = FILE_ATTRIBUTE_NORMAL;
    if ((attributes & gpg::EFileAttributes::FA_DeleteOnClose) != 0) {
        flags |= FILE_FLAG_DELETE_ON_CLOSE;
    }
    HANDLE file = ::CreateFileW(
        fileStr.c_str(),
        access,
        share,
        NULL,
        sCreationDisposition[attributes & 7],
        flags,
        NULL
    );
    this->mHandle = file;
    if (this->mHandle == INVALID_HANDLE_VALUE) {
        throw gpg::FileStream::IOError(::GetLastError());
    }
    this->mAccessKind = accessKind;
}

// 0x00955A80
void gpg::FileStream::DoWrite(const char *buff, size_t bytes) {
    if (this->CanRead()) {
        LARGE_INTEGER dist;
        dist.QuadPart = this->LeftToRead();
        if (! ::SetFilePointerEx(this->mHandle, dist, nullptr, FILE_CURRENT)) {
            throw gpg::FileStream::IOError{::GetLastError()};
        }
    }
    this->mReadEnd = nullptr;
    this->mReadPtr = nullptr;
    this->mReadStart = nullptr;
    if (bytes == 0) {
        DWORD written;
        if (! ::WriteFile(this->mHandle, buff, bytes, &written, 0)) {
            throw gpg::FileStream::IOError{*::_errno()};
        }
        GPG_ASSERT(written == bytes); // if (written != bytes) { gpg::HandleAssertFailure("written == bytes", 288, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\streams\\FileStream.cpp"); }
    }
    this->mWritePtr = this->mBuff.begin();
    this->mWriteStart = this->mBuff.begin();
    this->mWriteEnd = this->mBuff.end();
}

// 0x00955B60
size_t gpg::FileStream::DoRead(char *buff, size_t bytes) {
    if (this->SomethingWritten()) {
        this->DoWrite(this->mWriteStart, this->BytesWritten());
    }
    this->mWriteEnd = nullptr;
    this->mWriteStart = nullptr;
    this->mWritePtr = nullptr;
    DWORD read;
    if (! ::ReadFile(this->mHandle, buff, bytes, &read, 0)) {
        throw gpg::FileStream::IOError{::GetLastError()};
    }
    return read;
}

// 0x00957950
std::string gpg::GetWin32ErrorString(int id) {
    LPWSTR buffer;
    int res = ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &buffer, 0, 0);
    if (res == 0) {
        return std::string{gpg::STR_Printf("Unknown error 0x%08x", id)};
    } else {
        std::string ret{gpg::STR_WideToUtf8(buffer)};
        ::LocalFree(buffer);
        return ret;
    }
}
