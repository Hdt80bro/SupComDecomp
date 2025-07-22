#include "gpgcore/DatListItem.h"
#include "gpggal/TextureContext.h"
#include "gpggal/TextureD3D9.h"

namespace Moho {

class RD3DTextureResource :
    public gpg::DatListItem<Moho::RD3DTextureResource>
{
    gpg::gal::TextureContext context;
    boost::shared_ptr<gpg::gal::TextureD3D9> baseTex;
};

}