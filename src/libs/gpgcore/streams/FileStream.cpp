
#include <Windows.h>
#include "FileStream.h"
#include "gpgcore/String.h"

int dwCreationDisposition[] = {
    CREATE_NEW,
    OPEN_ALWAYS,
    OPEN_EXISTING,
    TRUNCATE_EXISTING,
    0,
    0,
    0,
    0
};


gpg::FileStream::IOError::IOError(int id)
    : std::runtime_error{gpg::FileErrorToString(id)}
{
    this->id = id;
}


gpg::FileStream::~FileStream() {
    this->Close(this->accessKind);
}



gpg::FileStream::FileStream(const char *filename, int accessKind, unsigned int attributes, int buffSize) :
    gpg::Stream{},
    handle{0},
    accessKind{0}
{
    this->buffer = gpg::AllocMemBuffer(buffSize);
    this->OpenFile(filename, accessKind, attributes);
}


void gpg::FileStream::OpenFile(const char *file, int accessKind, unsigned int attributes) {
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