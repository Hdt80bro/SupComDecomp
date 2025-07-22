#include "HeightField.h"
#include <algorithm>

// 0x004783D0
void Moho::CHeightField::InitField(int width, int height) {
    this->grid.width = width;
    this->grid.height = height;
    this->grid.data = (__int16 *)operator new(2 * height * width);
    for (int i = 0; i < this->grid.width * this->grid.height; ++i) {
        this->grid.data[i] = 0;
    }
}

// 0x00475DF0
Wm3::AxisAlignedBox3f Moho::CHeightField::GetTierBox(int x,int z, char tier) {
    Moho::CHeightField *v5; // ebp
    float minY; // xmm5_4
    float maxY; // xmm6_4
    float v10; // xmm1_4
    float v11; // xmm4_4
    float v12; // xmm7_4
    float minZ; // xmm3_4
    float v14; // xmm0_4
    float maxX; // xmm2_4
    float minX; // xmm0_4
    Wm3::AxisAlignedBox3f *result; // eax
    float maxZ; // xmm0_4
    float var_4; // [esp+Ch] [ebp-4h]
    float v21; // [esp+14h] [ebp+4h] SPLIT

    Moho::SMinMax<unsigned short> v20 = this->GetTierBoundsUWord((unsigned __int8)tier, x, z);
    minY = (float)v20.min * 0.0078125;
    maxY = (float)v20.max * 0.0078125;
    var_4 = (float)(this->grid.width - 1);
    v10 = (float)(1 << tier);
    v11 = (float)(z << tier);
    v12 = (float)(x << tier);
    minZ = (float)(this->grid.height - 1);
    v14 = v11 + v10;
    maxX = v12 + v10;
    if ( minZ <= (float)(v11 + v10) )
        v14 = (float)(this->grid.height - 1);
    v21 = v14;
    if ( v14 < 0.0 )
        v21 = 0.0;
    if ( minZ > v11 )
        minZ = (float)(z << tier);
    if ( minZ < 0.0 )
        minZ = 0.0;
    minX = var_4;
    if ( var_4 <= maxX )
        maxX = var_4;
    if ( maxX < 0.0 )
        maxX = 0.0;
    if ( var_4 > v12 )
        minX = (float)(x << tier);
    if ( minX < 0.0 )
        minX = 0.0;
    maxZ = v21;
    return {minX, minY, minZ, maxX, maxY, maxZ};
}

// 0x00475BF0
Moho::SMinMax<unsigned short> Moho::CHeightField::GetTierBoundsUWord(int idx, int x, int z) {
    if (idx <= 0) {
        unsigned short a = this->GetHeightAt(std::clamp(x,     0, this->grid.width - 1), std::clamp(z,     0, this->grid.height - 1));
        unsigned short b = this->GetHeightAt(std::clamp(x + 1, 0, this->grid.width - 1), std::clamp(z,     0, this->grid.height - 1));
        unsigned short c = this->GetHeightAt(std::clamp(x,     0, this->grid.width - 1), std::clamp(z + 1, 0, this->grid.height - 1));
        unsigned short d = this->GetHeightAt(std::clamp(x + 1, 0, this->grid.width - 1), std::clamp(z + 1, 0, this->grid.height - 1));
        return {std::max({a, b, c, d}), std::min({a, b, c, d})};
    } else {
        struct_iGrid *subgrid = &this->grids[idx - 1];
        return subgrid->data1.Get(std::clamp(x, 0, subgrid->data1.width - 1), std::clamp(z, 0, subgrid->data1.height - 1));
    }
}
