#include "user/WldProps.h"
#include "user/WldTerrain.h"

namespace Moho {

struct CWldMap
{
    Moho::RWldMapPreviewChunk *mMapPreviewChunk;
    Moho::IWldTerrainRes *mTerrainRes;
    Moho::CWldProps *mProps;
};

}
