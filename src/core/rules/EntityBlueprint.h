#include "core/rules/Blueprint.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

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

enum ECollisionShape
{
    COLSHAPE_None = 0x0,
    COLSHAPE_Box = 0x1,
    COLSHAPE_Sphere = 0x2,
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

class REntityBlueprint : public Moho::RBlueprint
{
    std::vector<std::string> mCategories;
    std::string mScriptModule;
    std::string mScriptClass;
    Moho::ECollisionShape mCollisionShape;
    float mSizeX;
    float mSizeY;
    float mSizeZ;
    float mAverageDensity;
    float mInertiaTensorX;
    float mInertiaTensorY;
    float mInertiaTensorZ;
    float mCollisionOffsetX;
    float mCollisionOffsetY;
    float mCollisionOffsetZ;
    int mDesiredShooterCap;
    Moho::SFootprint mFootprint;
    Moho::SFootprint mAltFootprint;
    bool mLifeBarRender;
    float mLifeBarOffset;
    float mLifeBarSize;
    float mLifeBarHeight;
    float mSelectionSizeX;
    float mSelectionSizeY;
    float mSelectionSizeZ;
    float mSelectionCenterOffsetX;
    float mSelectionCenterOffsetY;
    float mSelectionCenterOffsetZ;
    float mSelectionYOffset;
    float mSelectionMeshScaleX;
    float mSelectionMeshScaleY;
    float mSelectionMeshScaleZ;
    float mSelectionMeshUseTopAmount;
    float mSelectionThickness;
    float mUseOOBTestZoom;
    std::string mStrategicIconName;
    char mStrategicIconSortPriority;
    boost::shared_ptr<unk_t> mStrategicIconRest;
    boost::shared_ptr<unk_t> mStrategicIconSelected;
    boost::shared_ptr<unk_t> mStrategicIconOver;
    boost::shared_ptr<unk_t> mStrategicIconSelectedOver;
};

}
