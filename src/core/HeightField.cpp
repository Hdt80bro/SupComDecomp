#include "HeightField.h"
#include <algorithm>

// 0x004783D0
void Moho::CHeightField::InitField(int width, int height) {
    this->mGrid.mWidth = width;
    this->mGrid.mHeight = height;
    this->mGrid.mData = (__int16 *) operator new(2 * height * width);
    for (int i = 0; i < this->mGrid.mWidth * this->mGrid.mHeight; ++i) {
        this->mGrid.mData[i] = 0;
    }
}

// 0x00475DF0
Wm3::AxisAlignedBox3f Moho::CHeightField::GetTierBox(int x,int z, char tier) {
    Moho::SMinMax<unsigned short> v20 = this->GetTierBoundsUWord(tier, x, z);
    float minY = (float) v20.mMin * 0.0078125;
    float maxY = (float) v20.mMax * 0.0078125;
    float v10 = (float) (1 << tier);
    float v11 = (float) (z << tier);
    float v12 = (float) (x << tier);
    
    float minZ = (float) (this->mGrid.mHeight - 1);
    float maxZ = v11 + v10;
    maxZ = std::clamp(maxZ, 0.0f, minZ);
    minZ = std::clamp(minZ, 0.0f, v11);

    float minX = (float) (this->mGrid.mWidth - 1);
    float maxX = v12 + v10;
    maxX = std::clamp(maxX, 0.0f, minX);
    minX = std::clamp(minX, 0.0f, v12);
    return {minX, minY, minZ, maxX, maxY, maxZ};
}

// 0x00475BF0
Moho::SMinMax<unsigned short> Moho::CHeightField::GetTierBoundsUWord(int idx, int x, int z) {
    if (idx <= 0) {
        unsigned short a = this->GetHeightAt(std::clamp(x,     0, this->mGrid.mWidth - 1), std::clamp(z,     0, this->mGrid.mHeight - 1));
        unsigned short b = this->GetHeightAt(std::clamp(x + 1, 0, this->mGrid.mWidth - 1), std::clamp(z,     0, this->mGrid.mHeight - 1));
        unsigned short c = this->GetHeightAt(std::clamp(x,     0, this->mGrid.mWidth - 1), std::clamp(z + 1, 0, this->mGrid.mHeight - 1));
        unsigned short d = this->GetHeightAt(std::clamp(x + 1, 0, this->mGrid.mWidth - 1), std::clamp(z + 1, 0, this->mGrid.mHeight - 1));
        return {std::max({a, b, c, d}), std::min({a, b, c, d})};
    } else {
        struct_iGrid *subgrid = &this->mGrids[idx - 1];
        return subgrid->mData1.Get(std::clamp(x, 0, subgrid->mData1.mWidth - 1), std::clamp(z, 0, subgrid->mData1.mHeight - 1));
    }
}
