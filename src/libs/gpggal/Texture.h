#include <string>
#include "gpgcore/MemBuffer.h"

namespace gpg::gal {

// 0x00D42190
class TextureContext
{
public:
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

    virtual ~TextureContext() = default; // 0x008E7AE0
};

// 0x00D43AFC
class Texture
{
public:
    virtual ~Texture() = 0;
    virtual gpg::gal::TextureContext * GetContext() = 0;
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    virtual void Func1() = 0;
    virtual void SaveToBuffer() = 0;
};

}
