#include <stdexcept>
#include "boost/weak_ptr.hpp"
#include "gpgcore/streams/Stream.h"
#include "gpgcore/MemBuffer.h"



static int sCreationDisposition[8]; // 0x00D494B0
static int sMoveMethod[3];// 0x00D4953C

namespace gpg {


enum EFileAttributes
{
    // Creation disposition
    FA_CreateNew = 0,        // CREATE_NEW
    FA_OpenAlways = 1,       // OPEN_ALWAYS
    FA_OpenExisting = 2,     // OPEN_EXISTING
    FA_TruncateExisting = 3, // TRUNCATE_EXISTING
    // Share mode
    FA_ShareRead = 8,   // FILE_SHARE_READ
    FA_ShareWrite = 16, // FILE_SHARE_WRITE
    // Flags
    FA_DeleteOnClose = 32, // FILE_FLAG_DELETE_ON_CLOSE
};

// 0x00D4947C
class FileStream : public gpg::Stream
{
public:
    // 0x00D494A8
    class IOError : public std::runtime_error
    {
    public:
        int mId;

        IOError(int id); // 0x00955890
    };

public:
    HANDLE mHandle;
    gpg::Stream::Mode mAccessKind;
    gpg::MemBuffer<char> mBuff;

    ~FileStream() noexcept override; // 0x00955870
    size_t VirtTell(gpg::Stream::Mode mode) override; // 0x00955CE0
    size_t VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, __int64 pos) override; // 0x00955DF0
    size_t VirtRead(char *buff, size_t len) override; // 0x00955F80
    bool VirtAtEnd() override; // 0x009560C0
    void VirtWrite(const char *data, size_t size) override; // 0x00956180
    void VirtFlush() override; // 0x00956290
    void VirtClose(gpg::Stream::Mode mode) override; // 0x00956320

    FileStream(const char *filename, gpg::Stream::Mode accessKind, gpg::EFileAttributes attributes, size_t buffSize); // 0x00955BD0
    void DoOpen(const char *file, gpg::Stream::Mode accessKind, gpg::EFileAttributes attributes); // 0x00955990
    void DoWrite(const char *buff, size_t len); // 0x00955A80
    size_t DoRead(char *buff, size_t len); // 0x00955B60
};

std::string GetWin32ErrorString(int id); // 0x00957950

}
