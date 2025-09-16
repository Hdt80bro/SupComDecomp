#include "STIMap.h"

// 0x00577F20
bool Moho::STIMap::IsBlockingTerrain(unsigned int x, unsigned int y) {
  return y >= this->mHeightField->mGrid.mWidth - 1
      || x >= this->mHeightField->mGrid.mHeight - 1
      || this->mBlocking[(unsigned __int8)this->mTerrainType.Get(x, y)];
}

// 0x00577EC01
void Moho::STIMap::SetTerrainType(unsigned int x, unsigned int z, char type) {
    if (x < this->mHeightField->mGrid.mWidth - 1 && z < this->mHeightField->mGrid.mHeight - 1) {
        *this->mTerrainType.At(x, z) = type;
    }
}

// 0x00758E10
LuaPlus::LuaObject Moho::STIMap::GetTerrainType(unsigned int x, unsigned int z) {
    int k = 1;
    if (x < this->mHeightField->mGrid.mWidth - 1 && z < this->mHeightField->mGrid.mHeight - 1) {
        k = this->mTerrainType.Get(x, z);
    }
    return this->mTerrainTypes.mTTVec[k];
}

// 0x00758E90
float Moho::STIMap::GetTerrainTypeOffset(float x, float z) {
    LuaPlus::LuaObject tt = this->GetTerrainType((int) x, (int) z);
    if (tt.IsTable()) {
        LuaPlus::LuaObject offset = tt["HeightOffset"];
        if (! offset.IsNil()) {
            return offset.GetNumber();
        }
    }
    return 0.0;
}
