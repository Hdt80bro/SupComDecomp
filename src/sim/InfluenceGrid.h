#include <map>
#include <vector>

namespace Moho {

struct SThreat
{
    float overallInfluence;
    float influenceStructuresNotMex;
    float influenceStructures;
    float navalInfluence;
    float airInfluence;
    float landInfluence;
    float experimentalInfluence;
    float commanderInfluence;
    float artilleryInfluence;
    float antiAirInfluence;
    float antiSurfaceInfluence;
    float antiSubInfluence;
    float economyInfluence;
    float unknownInfluence;
};

struct InfluenceMapEntry
{
    Moho::EntId id;
    Moho::Sim *sim;
    Wm3::Vector3f pos;
    Moho::RUnitBlueprint *bp;
    Moho::ELayer layer;
    bool v7;
    float v8;
    float v9;
    int v10;
    int v11;
};

struct InfluenceGrid
{
    std::map<unsigned int, Moho::InfluenceMapEntry> entries;
    std::vector<Moho::SThreat> threats;
    Moho::SThreat threat;
    Moho::SThreat decay;
};

}
