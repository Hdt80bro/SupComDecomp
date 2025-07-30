#include "d3d9helper.h"

namespace gpg::gal {

class StateCache
{
public:
    int key;
    int value;
    int v1;
};


class StateManagerD3D9
{
public:
    volatile LONG uses;
    IDirect3DDevice9 *device;
    gpg::gal::StateCache renderStateCache;
    gpg::gal::StateCache samplerStateCache[16];
    gpg::gal::StateCache textureStageStateCache[8];
    IDirect3DVertexShader9 *vertexShader;
    IDirect3DPixelShader9 *pixelShader;
    int fvf;
};

}
