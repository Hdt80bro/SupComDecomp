#include "d3d9helper.h"
#include "gpggal/Texture.h"

namespace gpg::gal {

// 0x00D481D4
class TextureD3D9 : public gpg::gal::Texture
{
public:
    gpg::gal::TextureContext context;
    IDirect3DTexture9 *texture;
    bool locking;
    int level;

    ~TextureD3D9() override = default; // 0x0094AB60
    gpg::gal::TextureContext *GetContext() override; // 0x0094A080
    void Lock() override; // 0x0094A150
    void Unlock() override; // 0x0094A410
    void Func1() override; // 0x0094A090
    void SaveToBuffer() override; // 0x0094A630
};

}
