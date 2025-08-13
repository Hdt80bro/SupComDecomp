#include "IntelGrid.h"

// 0x00506E20
Moho::GridPos(Wm3::Vector3f *wldPos, int gridSize) {
    this->x = wldPos->x / gridSize;
    this->z = wldPos->z / gridSize;
}

// 0x00507540
void Moho::CIntelGrid::Raster(Wm3::Vector3f *worldPos, int gridRadius, bool doAdd) {
    Moho::GridPos gridPos(worldPos, this->mGridSize);
    
    int x =    std::clamp(gridPos.x - gridRadius, 0, this->mGrid.mSizeX);
    int xMax = std::clamp(gridPos.x + gridRadius, 0, this->mGrid.mSizeX);

    for ( ; x < xMax; ++x) {
        int cosine = gridPos.x - x;
        int sine = (int) sqrtf(gridRadius*gridRadius - cosine*cosine);

        int z =    std::clamp(gridPos.z - sine, 0, this->mGrid.mSizeY);
        int zMax = std::clamp(gridPos.z + sine, 0, this->mGrid.mSizeY);
        for ( ; z < zMax; ++z) {
            this->mGrid.at(x, z) += doAdd ? +1 : -1;
        }
    }
}
