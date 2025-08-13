
// 0x00715FF0
float Moho::CInfluenceMap::GetThreatRect(
    int x,
    int z,
    int radius,
    bool onMap,
    Moho::EThreatType threatType,
    int army)
{
    Moho::STIMap *mapData = this->mArmy->GetSim()->mMapData;
    gpg::Rect2i border{
        mapData->mPlayableRect.x0 / this->mGridSize,
        mapData->mPlayableRect.z0 / this->mGridSize,
        mapData->mPlayableRect.x1 / this->mGridSize,
        mapData->mPlayableRect.z1 / this->mGridSize
    };
    int maxZ = z + radius;
    if (z - radius > z + radius) {
        return 0.0;
    }
    float totalThreat = 0.0;
    for (int curZ = z - radius; curZ <= maxZ; ++curZ) {
        if (curZ >= 0 && curZ < this->mHeight && (!onMap || curZ >= border.z0 && curZ <= border.z1)) {
            int maxX = x + radius;
            for (int curX = x - radius; curX <= maxX; ++curX) {
                if (curX >= 0 && curX < this->mWidth && (!onMap || curX >= border.x0 && curX <= border.x1)) {
                    totalThreat += this->mMapEntries.start[curX + curZ * this->mWidth]->GetThreat(threatType, army);
                }
            }
        }
    }
    return totalThreat;
}