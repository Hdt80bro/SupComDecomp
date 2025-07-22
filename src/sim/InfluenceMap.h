#include <vector>
#include <map>

namespace Moho {

struct CInfluenceMap
{
    Moho::CArmyImpl *army;
    int total;
    int width;
    int height;
    int gridSize;
    std::map<uint, int> v5;
    std::vector<Moho::InfluenceGrid> mapEntries;

    float GetThreatRect(
        int x,
        int z,
        int radius,
        bool onMap,
        Moho::EThreatType threatType,
        int army); // 0x00715FF0
};

}
