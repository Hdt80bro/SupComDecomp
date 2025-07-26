#include "Stream.h"
#include "boost/weak_ptr.hpp"


static int dwCreationDisposition[]; // 0x00D494B0
static int dwMoveMethod[];// 0x00D4953C

namespace gpg {

class FileStream : public gpg::Stream
{
public:
    HANDLE handle;
    gpg::Stream::Mode accessKind;
    gpg::MemBuffer<char> buff;

    class IOError : public std::runtime_error
    {
    public:
        int id;

        IOError(int id); // 0x00955890
    };

    ~FileStream() noexcept override; // 0x00955870
    size_t VirtTell(gpg::Stream::Mode mode) override; // 0x00955CE0
    size_t VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, size_t pos) override; // 0x00955DF0
    size_t VirtRead(char *buf, size_t len) override; // 0x00955F80
    bool VirtAtEnd() override; // 0x009560C0
    void VirtWrite(const char *data, size_t size) override; // 0x00956180
    void VirtFlush() override; // 0x00956290
    void VirtClose(gpg::Stream::Mode mode) override; // 0x00956320

    FileStream(const char *filename, gpg::Stream::Mode accessKind, unsigned int attributes, size_t buffSize); // 0x00955BD0
    void OpenFile(const char *file, gpg::Stream::Mode accessKind, unsigned int attributes); // 0x00955990
    size_t ReadFile(char *buf, size_t len); // 0x00955B60
    void Flush(const char *buf, size_t len); // 0x00955A80
};

std::string FileErrorToString(int id); // 0x00957950

}
