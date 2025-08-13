#include "core/IntelUpdate.h"
#include "core/STIMap.h"
#include "Wm3Vector3.h"
#include <vector>

namespace Moho {

struct GridPos
{
    int x;
    int z;

    GridPos(Wm3::Vector3f *wldPos, int gridSize); // 0x00506E20
};

struct CIntelGrid
{
    Moho::STIMap *mMapData;
    gpg::Array2D mGrid;
    std::vector<Moho::CIntelUpdate> mUpdateList;
    int mGridSize;

    void Raster(Wm3::Vector3f *worldPos, int gridRadius, bool doAdd); // 0x00507540


    inline void SubtractCircle(Wm3::Vector3f *pos, unsigned int radius) {
        this->Raster(pos, radius / this->mGridSize, false);
    } // 0x00507690
    
    inline void AddCircle(Wm3::Vector3f *pos, unsigned int radius) {
        this->Raster(pos, radius / this->mGridSize, true);
    } // 0x00507670
};

}
