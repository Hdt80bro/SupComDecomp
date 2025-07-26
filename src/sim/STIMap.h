#include "LuaPlus.h"
#include "boost/shared_ptr.hpp"
#include "gpgcore/containers/fastvector.h"
#include "gpgcore/containers/Array2D.h"
#include "gpgcore/containers/Rect.h"
#include "sim/HeightField.h"

namespace Moho {

struct TerrainTypes
{
    gpg::fastvector_n<LuaPlus::LuaObject, 256> ttvec;
};

struct STIMap
{
    boost::shared_ptr<Moho::CHeightField> heightField;
    gpg::Rect2i playableRect;
    Moho::TerrainTypes terrainTypes;
    gpg::Array2D terrainType;
    bool blocking[256];
    bool waterEnabled;
    float waterElevation;
    float waterElevationDeep;
    float waterElevationAbyss;
    int v1;

    bool IsBlockingTerrain(unsigned int x, unsigned int z); // 0x00577F20
    void SetTerrainType(unsigned int x, unsigned int z, char type); // 0x00577EC01
    LuaPlus::LuaObject GetTerrainType(unsigned int x, unsigned int z); // 0x00758E10
    float GetTerrainTypeOffset(float x, float z); // 0x00758E90
};
    
}
