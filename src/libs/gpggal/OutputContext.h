#include "boost/shared_ptr.hpp"
#include "gpggal/CubeRenderTargetD3D9.h"

namespace gpg::gal {

// 0x00D42180
class OutputContext
{
public:
    boost::shared_ptr<gpg::gal::CubeRenderTargetD3D9> cubeTarget;
    int face;
    boost::shared_ptr<gpg::gal::D3DSurface> surface;
    boost::weak_ptr<int> obj2;

    virtual ~OutputContext() = default; // 0x008E8250
};


}