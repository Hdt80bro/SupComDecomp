#include <list>
#include <string>

namespace Moho {

enum EOccupancyCaps : char
{
    OC_LAND = 0x1,
    OC_SEABED = 0x2,
    OC_SUB = 0x4,
    OC_WATER = 0x8,
    OC_AIR = 0x10,
    OC_ORBIT = 0x20,
    OC_ANY = 0xFF,
};

enum EFootprintFlags : char
{
    FPFLAG_None = 0x0,
    FPFLAG_IgnoreStructures = 0x1,
};

struct SFootprint
{
    unsigned __int8 mSizeX;
    unsigned __int8 mSizeZ;
    Moho::EOccupancyCaps mOccupancyCaps;
    Moho::EFootprintFlags mFlags;
    float mMaxSlope;
    float mMinWaterDepth;
    float mMaxWaterDepth;
};

struct SNamedFootprint : Moho::SFootprint
{
    std::string mName;
    int mIndex;
};

struct SRuleFootprintsBlueprint
{
    std::list<Moho::SNamedFootprint> mFootprints;
    int likelyGap;
};

}
