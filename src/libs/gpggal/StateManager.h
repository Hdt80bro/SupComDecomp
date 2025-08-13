#include "d3d9helper.h"

namespace gpg::gal {

class StateCache
{
public:
    int mKey;
    int mValue;
    int v1;
};


class StateManagerD3D9
{
public:
    volatile LONG mUses;
    IDirect3DDevice9 *mDevice;
    gpg::gal::StateCache mRenderStateCache;
    gpg::gal::StateCache mSamplerStateCache[16];
    gpg::gal::StateCache mTextureStageStateCache[8];
    IDirect3DVertexShader9 *mVertexShader;
    IDirect3DPixelShader9 *mPixelShader;
    int fvf;
};

}
