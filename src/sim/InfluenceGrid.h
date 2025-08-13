#include <map>
#include <vector>

namespace Moho {

struct SThreat
{
    float mOverallInfluence;
    float mInfluenceStructuresNotMex;
    float mInfluenceStructures;
    float mNavalInfluence;
    float mAirInfluence;
    float mLandInfluence;
    float mExperimentalInfluence;
    float mCommanderInfluence;
    float mArtilleryInfluence;
    float mAntiAirInfluence;
    float mAntiSurfaceInfluence;
    float mAntiSubInfluence;
    float mEconomyInfluence;
    float mUnknownInfluence;
};

struct InfluenceMapEntry
{
    Moho::EntId mId;
    Moho::Sim *mSim;
    Wm3::Vector3f mPos;
    Moho::RUnitBlueprint *mBlueprint;
    Moho::ELayer mLayer;
    bool v7;
    float v8;
    float v9;
    int v10;
    int v11;
};

struct InfluenceGrid
{
    std::map<unsigned int, Moho::InfluenceMapEntry> mEntries;
    std::vector<Moho::SThreat> mThreats;
    Moho::SThreat mThreat;
    Moho::SThreat mDecay;
};

}
