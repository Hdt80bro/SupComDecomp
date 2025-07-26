#include "gpgcore/streams/Stream.h"
#include "gpgcore/MemBuffer.h"

struct MemBufferStream : gpg::Stream
{
    gpg::MemBuffer<char> input;
    gpg::MemBuffer<char> output;

    ~MemBufferStream() noexcept override; // 0x008E5B80

    MemBufferStream(unsigned int size); // 0x008E5AE0
    unsigned int GetLength(); // 0x008E59F0
    gpg::MemBuffer<const char> GetConstBuffer(); // 0x0088B7E0
};
