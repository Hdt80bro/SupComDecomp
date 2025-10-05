#include "gpgcore/streams/Stream.h"
#include <stdexcept>

namespace gpg {

struct BinaryReader
{
    // 0x00E028CC
    class PrematureEOF : std::runtime_error
    {
    public:
        PrematureEOF() : std::runtime_error{std::string{"Premature EOF"}} {} // 0x0043D180
    };

    gpg::Stream *mStream;

    void Read(void *data, size_t size); // 0x0043D210

    template<class T>
    void Read(T &out) {
        this->Read((void *) &out, sizeof(out));
    }
};

}
