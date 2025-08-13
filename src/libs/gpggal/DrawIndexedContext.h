#include "d3d9helper.h"

namespace gpg::gal {

// 0x00D47B38
class DrawIndexedContext
{
public:
    D3DPRIMITIVETYPE mPrimType;
    int mMinVertIndex;
    int mNumVertices;
    int mPrimCount;
    int mStartIndex;
    int mBaseVertIndex;

    virtual ~DrawIndexedContext() = default; // 0x0093F160
};

}

