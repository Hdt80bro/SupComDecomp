#include "boost/shared_ptr.hpp"
#include "LuaPlus.h"
#include "gpgcore/containers/Array2D.h"
#include "gpgcore/containers/fastvector.h"
#include "gpgcore/containers/Rect.h"
#include "core/HeightField.h"

namespace Moho {

struct TerrainTypes
{
    gpg::fastvector_n<LuaPlus::LuaObject, 256> mTTVec;
};

struct STIMap
{
    boost::shared_ptr<Moho::CHeightField> mHeightField;
    gpg::Rect2i mPlayableRect;
    Moho::TerrainTypes mTerrainTypes;
    gpg::Array2D mTerrainType;
    bool mBlocking[256];
    bool mWaterEnabled;
    float mWaterElevation;
    float mWaterElevationDeep;
    float mWaterElevationAbyss;
    int v1;

    bool IsBlockingTerrain(unsigned int x, unsigned int z); // 0x00577F20
    void SetTerrainType(unsigned int x, unsigned int z, char type); // 0x00577EC01
    LuaPlus::LuaObject GetTerrainType(unsigned int x, unsigned int z); // 0x00758E10
    float GetTerrainTypeOffset(float x, float z); // 0x00758E90
};
    
}
