#include "d3d9helper.h"

namespace gpg::gal {

// 0x00D47B38
class DrawIndexedContext
{
public:
    D3DPRIMITIVETYPE primType;
    int minVertIndex;
    int numVertices;
    int primCount;
    int startIndex;
    int baseVertIndex;

    virtual ~DrawIndexedContext() = default; // 0x0093F160
};

}

