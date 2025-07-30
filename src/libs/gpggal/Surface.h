#include "boost/shared_ptr.hpp"

namespace gpg::gal {

class D3DSurface
{
public:
    int v1;
    int v2;
    int v3;
    int v4;
    boost::shared_ptr<Moho::MeshBatch> obj1;
};

}
