#include "d3d9helper.h"
#include "gpggal/CubeRenderTarget.h"

namespace gpg::gal {

// 0x00D47CB4
class CubeRenderTargetD3D9 : public gpg::gal::CubeRenderTarget
{
public:
    gpg::gal::CubeRenderTargetContext mContext;
    int v4;
    IDirect3DSurface9 *mFaces[6];

    ~CubeRenderTargetD3D9() override = default; // 0x00941430
};


}
