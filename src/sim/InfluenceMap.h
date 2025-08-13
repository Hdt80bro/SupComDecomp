#include <vector>
#include <map>
#include "sim/InfluenceGrid.h"

namespace Moho {

enum EThreatType
{
    THREATTYPE_Overall = 0x0,
    THREATTYPE_OverallNotAssigned = 0x1,
    THREATTYPE_StructuresNotMex = 0x2,
    THREATTYPE_Structures = 0x3,
    THREATTYPE_Naval = 0x4,
    THREATTYPE_Air = 0x5,
    THREATTYPE_Land = 0x6,
    THREATTYPE_Experimental = 0x7,
    THREATTYPE_Commander = 0x8,
    THREATTYPE_Artillery = 0x9,
    THREATTYPE_AntiAir = 0xA,
    THREATTYPE_AntiSurface = 0xB,
    THREATTYPE_AntiSub = 0xC,
    THREATTYPE_Economy = 0xD,
    THREATTYPE_Unknown = 0xE,
};


struct CInfluenceMap
{
    Moho::IArmy *mArmy;
    int mTotal;
    int mWidth;
    int mHeight;
    int mGridSize;
    std::map<unsigned int, int> v5;
    std::vector<Moho::InfluenceGrid> mMapEntries;

    float GetThreatRect(
        int x,
        int z,
        int radius,
        bool onMap,
        Moho::EThreatType threatType,
        int army); // 0x00715FF0
};

}
