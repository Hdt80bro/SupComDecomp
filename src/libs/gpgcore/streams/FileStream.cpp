#include "FileStream.h"
#include <Windows.h>
#include "gpgcore/String.h"
#include "gpgcore/MemBuffer.h"

int dwCreationDisposition[] = { // 0x00D494B0
    CREATE_NEW,
    OPEN_ALWAYS,
    OPEN_EXISTING,
    TRUNCATE_EXISTING,
    0,
    0,
    0,
    0,
};
int dwMoveMethod[] = { // 0x00D4953C
    FILE_BEGIN,
    FILE_CURRENT,
    FILE_END,
};

// 0x00955890
gpg::FileStream::IOError::IOError(int id)
    : std::runtime_error{gpg::FileErrorToString(id)}
{
    this->id = id;
}

// 0x00955870
gpg::FileStream::~FileStream() {
    this->Close(this->accessKind);
}

// 0x00955CE0
size_t gpg::FileStream::VirtTell(gpg::Stream::Mode mode) {
    if ((mode & this->accessKind) == 0) {
        throw std::invalid_argument{std::string{"Invalid mode for Tell()"}};
    }
    LARGE_INTEGER Size;
    Size.QuadPart = 0;
    LARGE_INTEGER NewFilePointer;
    if (! SetFilePointerEx(this->handle, Size, &NewFilePointer, 1)) {
        throw gpg::FileStream::IOError{GetLastError()};
    }
    size_t pos = NewFilePointer.LowPart;
    if (this->end != this->readHead) {
        pos -= this->end - this->readHead;
    }
    if (this->writeStart != this->writeHead) {
        pos += this->writeStart - this->writeHead;
    }
    return pos;
}

// 0x00955DF0
size_t gpg::FileStream::VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, size_t pos) {
    if ((this->accessKind & mode) == 0) {
        throw std::invalid_argument{std::string{"Invalid mode for Seek()"}};
    }
    if (orig > gpg::Stream::OriginEnd) {
        throw std::invalid_argument{std::string{"Invalid origin for Seek()"}};
    }
    if (this->writeStart != this->writeHead) {
        this->Flush(this->writeHead, this->writeStart - this->writeHead);
    }
    LARGE_INTEGER dist;
    dist.QuadPart = pos;
    if (this->end != this->readHead && orig == gpg::Stream::OriginCurr) {
        dist.QuadPart -= this->end - this->readHead;
    }
    LARGE_INTEGER NewFilePointer;
    if (! SetFilePointerEx(this->handle, dist, &NewFilePointer, dwMoveMethod[orig])) {
        throw gpg::FileStream::IOError{GetLastError()};
    }
    this->end = nullptr;
    this->readHead = nullptr;
    this->start = nullptr;
    this->dataEnd = nullptr;
    this->writeStart = nullptr;
    this->writeHead = nullptr;
    return NewFilePointer.QuadPart;
}

// 0x00956180
void gpg::FileStream::VirtWrite(const char *str, size_t len) {
    if ((this->accessKind & gpg::Stream::ModeSend) == 0) {
        throw std::runtime_error{std::string{"Attempt to write to a file that isn't open for output."}};
    }
    if (len > this->LeftInWriteBuffer()) {
        int size = this->LeftInWriteBuffer();
        if (size != 0 && len - size < this->buff.Size()) {
            memcpy(this->writeStart, str, size);
            this->writeStart += size;
            str += size;
            len -= size;
        }
        this->Flush(this->writeHead, this->writeStart - this->writeHead);
        if (len >= this->buff.Size()) {
            this->Flush(str, len);
            return;
        }
    }
    memcpy(this->writeStart, str, len);
    this->writeStart += len;
}

// 0x00955F80
size_t gpg::FileStream::VirtRead(char *buf, size_t len) {
    if ((this->accessKind & gpg::Stream::ModeReceive) == 0) {
        throw std::runtime_error{std::string{"Attempt to read from a file that isn't open for input."}};
    }
    size_t readBuf = this->end - this->readHead;
    int readAmt = 0;
    while (readBuf < len) {
        if (readBuf != 0) {
            memcpy(&buf[readAmt], this->readHead, readBuf);
            this->readHead += readBuf;
            readAmt += readBuf;
            buf += readBuf;
            len -= readBuf;
        }
        if (len >= this->buff.Size()) {
            size_t amt = this->ReadFile(buf, len);
            if (amt) {
                readAmt += amt;
                buf += amt;
                len -= amt;
            } else {
                return readAmt;
            }
        } else {
            size_t amt = this->ReadFile(this->buff.begin, this->buff.Size());
            if (amt == 0) {
                return readAmt;
            }
            this->readHead = this->buff.begin;
            this->start = this->buff.begin;
            this->end = &this->buff.begin[amt];
        }
        readBuf = this->end - this->readHead;
    }
    memcpy(buf, this->readHead, len);
    this->readHead += len;
    readAmt += len;
    return readAmt;
}

// 0x009560C0
bool gpg::FileStream::VirtAtEnd() {
    if ((this->accessKind & gpg::Stream::ModeReceive) == 0) {
        throw std::runtime_error{std::string{"Attempt to read from a file that isn't open for input."}};
    }
    if (this->readHead != this->end) {
        return false;
    }
    size_t amt = this->ReadFile(this->buff.begin, this->buff.Size());
    this->readHead = this->buff.begin;
    this->start = this->buff.begin;
    this->end = &this->buff.begin[amt];
    return amt == 0;
}

// 0x00956290
void gpg::FileStream::VirtFlush() {
    if ((this->accessKind & gpg::Stream::ModeSend) == 0) {
        throw std::runtime_error{std::string{"Attempt to write to a file that isn't open for output."}};
    }
    if (this->writeStart != this->writeHead) {
        this->Flush(this->writeHead, this->writeStart - this->writeHead);
    }
}

// 0x00956320
void gpg::FileStream::VirtClose(gpg::Stream::Mode mode) {
    if ((this->accessKind & mode & gpg::Stream::ModeSend) != 0) {
        if (this->writeStart != this->writeHead) {
            this->Flush(this->writeHead, this->writeStart - this->writeHead);
        }
        this->accessKind &=~ gpg::Stream::ModeSend;
        this->dataEnd = nullptr;
        this->writeStart = nullptr;
        this->writeHead = nullptr;
    }
    if ((this->accessKind & mode & gpg::Stream::ModeReceive) != 0) {
        this->accessKind &=~ gpg::Stream::ModeReceive;
        this->end = nullptr;
        this->readHead = nullptr;
        this->start = nullptr;
    }
    if (*(_QWORD *)&this->handle < 0xFFFFFFFFuLL) {
        CloseHandle(this->handle);
        this->handle = (HANDLE)-1;
        gpg::MemBuffer::~MemBuffer(&this->buff);
    }
}


// 0x00955BD0
gpg::FileStream::FileStream(const char *filename, gpg::Stream::Mode accessKind, unsigned int attributes, size_t buffSize) :
    gpg::Stream{},
    handle{0},
    accessKind{gpg::Stream::ModeNone}
{
    this->buff = gpg::AllocMemBuffer(buffSize);
    this->OpenFile(filename, accessKind, attributes);
}

// 0x00955990
void gpg::FileStream::OpenFile(const char *file, gpg::Stream::Mode accessKind, unsigned int attributes) {
    std::wstring filename = gpg::STR_Utf8ToWide(file);
    HANDLE FileW = CreateFileW(
        filename.data(),
        ((4 * accessKind) | accessKind & 2) << 29,
        (attributes >> 3) & 3,
        0,
        dwCreationDisposition[attributes & 7],
        ((attributes & 0x20) << 21) | FILE_ATTRIBUTE_NORMAL,
        0);
    this->handle = FileW;
    if (this->handle == INVALID_HANDLE_VALUE) {
        throw gpg::FileStream::IOError(GetLastError());
    }
    this->accessKind = accessKind;
}

// 0x00955B60
size_t gpg::FileStream::ReadFile(char *buf, size_t len) {
    if (this->writeStart != this->writeHead) {
        this->Flush(this->writeHead, this->writeStart - this->writeHead);
    }
    this->dataEnd = nullptr;
    this->writeHead = nullptr;
    this->writeStart = nullptr;
    DWORD NumberOfBytesRead;
    if (! ::ReadFile(this->handle, buf, len, &NumberOfBytesRead, 0)) {
        throw gpg::FileStream::IOError{GetLastError()};
    }
    return NumberOfBytesRead;
}

// 0x00955A80
void gpg::FileStream::Flush(const char *buf, size_t bytes) {
    if (this->readHead != this->end) {
        LARGE_INTEGER dist;
        dist.QuadPart = this->readHead - this->end;
        if (! SetFilePointerEx(this->handle, dist, nullptr, FILE_CURRENT)) {
            throw gpg::FileStream::IOError{GetLastError()};
        }
    }
    this->end = nullptr;
    this->readHead = nullptr;
    this->start = nullptr;
    if (bytes == 0) {
        DWORD written;
        if (! WriteFile(this->handle, buf, bytes, &written, 0)) {
            throw gpg::FileStream::IOError{*_errno()};
        }
        // GPG_ASSERT(written == bytes);
        if (written != bytes) {
            gpg::HandleAssertFailure("written == bytes", 288, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\streams\\FileStream.cpp");
        }
    }
    this->writeStart = this->buff.begin;
    this->writeHead = this->buff.begin;
    this->dataEnd = this->buff.end;
}

// 0x00957950
std::string gpg::FileErrorToString(int id) {
    LPWSTR Buffer;
    int res = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER, 0, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &Buffer, 0, 0);
    if (res == 0) {
        return std::string{gpg::STR_Printf("Unknown error 0x%08x", id)};
    } else {
        std::string ret{gpg::STR_WideToUtf8(Buffer)};
        LocalFree(Buffer);
        return ret;
    }
}
