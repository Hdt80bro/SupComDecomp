#include "gpgcore/MemBuffer.h"
#include <string>

namespace Moho {

struct RScaResource
{
    std::string mFilename;
    gpg::MemBuffer<char> mMem;
    char *mStart;
    char *mEnd;
};

}
