#include <vector>

namespace gpg::gal {

// 0x00D42F08
class VertexBuffer
{
public:
    virtual ~VertexBuffer() = 0;
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
};

}
