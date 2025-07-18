
// 0x00715FF0
float Moho::CInfluenceMap::GetThreatRect(
    int x,
    int z,
    int radius,
    bool onMap,
    Moho::EThreatType threatType,
    int army)
{
    Moho::STIMap *mapData = this->army->GetSim(this->army)->mapData;
    gpg::Rect2i border {
        mapData->playableRect.x0 / this->gridSize,
        mapData->playableRect.z0 / this->gridSize,
        mapData->playableRect.x1 / this->gridSize,
        mapData->playableRect.z1 / this->gridSize
    };
    int maxZ = z + radius;
    if (z - radius > z + radius) {
        return 0.0;
    }
    float totalThreat = 0.0;
    for (int curZ = z - radius; curZ <= maxZ; ++curZ) {
        if (curZ >= 0 && curZ < this->height && (!onMap || curZ >= border.z0 && curZ <= border.z1)) {
            int maxX = x + radius;
            for (int curX = x - radius; curX <= maxX; ++curX) {
                if (curX >= 0 && curX < this->width && (!onMap || curX >= border.x0 && curX <= border.x1)) {
                    totalThreat += this->mapEntries.start[curX + curZ * this->width]->GetThreat(threatType, army);
                }
            }
        }
    }
    return totalThreat;
}