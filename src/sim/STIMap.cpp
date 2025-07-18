#include "STIMap.h"

// 0x00577F20
bool Moho::STIMap::IsBlockingTerrain(unsigned int x, unsigned int y) {
  return y >= this->heightField->width - 1
      || x >= this->heightField->height - 1
      || this->blocking[(unsigned __int8)this->terrainType.Get(x, y)];
}

// 0x00577EC01
void Moho::STIMap::SetTerrainType(unsigned int x, unsigned int z, char type) {
    if (x < this->heightField->width - 1 && z < this->heightField->height - 1) {
        *this->terrainType.At(x, z) = type;
    }
}

// 0x00758E10
LuaPlus::LuaObject Moho::STIMap::GetTerrainType(unsigned int x, unsigned int z) {
    int k = 1;
    if (x < this->heightField->width - 1 && z < this->heightField->height - 1) {
        k = this->terrainType.Get(x, z);
    }
    return this->terrainTypes.ttvec.start[k];
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
