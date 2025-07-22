#include <string>
#include "gpgcore/MemBuffer.h"

namespace gpg {
namespace gal {

class TextureContext
{
    int source;
    std::string location;
    gpg::MemBuffer<char> data;
    int type;
    int usage;
    int format;
    int mipmapLevels;
    int v17;
    int width;
    int height;
    int v20;
};

}
}