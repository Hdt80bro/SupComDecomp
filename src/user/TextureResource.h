#include "gpgcore/DatListItem.h"
#include "gpggal/Texture.h"
#include "gpggal/TextureD3D9.h"

namespace Moho {

// 0x00E0276C
class RD3DTextureResource :
    public gpg::DatListItem<Moho::RD3DTextureResource>
{
    gpg::gal::TextureContext mContext;
    boost::shared_ptr<gpg::gal::TextureD3D9> mBaseTex;
};

}