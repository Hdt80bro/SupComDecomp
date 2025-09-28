#include "gpgcore/streams/Stream.h"
#include "gpgcore/MemBuffer.h"


namespace gpg {

// 0x00D41E20
class MemBufferStream : public gpg::Stream
{
public:
    gpg::MemBuffer<char> mInput;
    gpg::MemBuffer<char> mOutput;

    ~MemBufferStream() noexcept override = default; // 0x008E5B80
    size_t VirtTell(gpg::Stream::Mode mode) override; // 0x0008E5DC0
    size_t VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, __int64 pos) override; // 0x0008E6140
    size_t VirtRead(char * buf, size_t len) override; // 0x008E5A50
    void VirtUnGetByte(int) override; // 0x008E5E70
    bool VirtAtEnd() override; // 0x008E5AB0
    void VirtWrite(const char *data, size_t size) override; // 0x008E6470
    void VirtFlush() override; // 0x008E5AD0

    gpg::MemBuffer<const char> GetConstBuffer(); // 0x0088B7E0
    size_t GetLength(); // 0x008E59F0
    void Resize(__int64 size); // 0x008E5EE0
    MemBufferStream(size_t size); // 0x008E5AE0
};

}
