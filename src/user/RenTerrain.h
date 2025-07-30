#include "boost/shared_ptr.hpp"
#include "TextureResource.h"

namespace Moho {

static int graphics_Fidelity; // 0x00F58E88
static int graphics_FidelitySupported; // 0x00F58E8C
static int shadow_Fidelity; // 0x00F58E90
static int shadow_FidelitySupported; // 0x00F58E94

// 0x00E41994
class IRenTerrain
{
public:
    virtual ~IRenTerrain() noexcept = default;
};

// 0x00E419D4
class TerrainCommon :
    public Moho::IRenTerrain
{
public:
    boost::shared_ptr<Moho::RD3DTextureResource> decalMask;

    ~TerrainCommon() noexcept override = default;
};

}
