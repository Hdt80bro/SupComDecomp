#include "gpg/streams/Stream.h"

namespace gpg {

struct TextWriter
{
    gpg::Stream *mStrm;
    int mState;

    void WriteNewline(); // 0x00957060
};

}
