#include "CIntelGrid.h"

Moho::GridPos(Wm3::Vector3f *wldPos, int gridSize) {
    this->x = wldPos->x / gridSize;
    this->z = wldPos->z / gridSize;
}

void Moho::CIntelGrid::Raster(Wm3::Vector3f *worldPos, int gridRadius, bool doAdd) {
    Moho::GridPos gridPos(worldPos, this->gridSize);
    
    int x =    clamp(gridPos.x - gridRadius, 0, this->width);
    int xMax = clamp(gridPos.x + gridRadius, 0, this->width);

    for (; x < xMax; ++x) {
        int cosine = gridPos.x - x;
        int sine = (int) sqrtf(gridRadius*gridRadius - cosine*cosine);

        int z =    clamp(gridPos.z - sine, 0, this->height);
        int zMax = clamp(gridPos.z + sine, 0, this->height);
        for (; z < zMax; ++z) {
            this->grid[x + z * this->width] += doAdd ? +1 : -1;
        }
    }
}