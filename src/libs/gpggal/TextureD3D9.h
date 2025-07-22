#include "TextureContext.h"
#include "d3d9helper.h"

namespace gpg {
namespace gal {

class TextureD3D9
{
    gpg::gal::TextureContext context;
    IDirect3DTexture9 *texture;
    bool locking;
    int level;
};

}
}