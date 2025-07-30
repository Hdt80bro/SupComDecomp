#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "core/reflection/Blueprint.h"

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
    unsigned __int8 SizeX;
    unsigned __int8 SizeZ;
    Moho::EOccupancyCaps OccupancyCaps;
    Moho::EFootprintFlags Flags;
    float MaxSlope;
    float MinWaterDepth;
    float MaxWaterDepth;
};


class REntityBlueprint : public Moho::RBlueprint
{
    std::vector<std::string> Categories;
    std::string ScriptModule;
    std::string ScriptClass;
    Moho::ECollisionShape CollisionShape;
    float SizeX;
    float SizeY;
    float SizeZ;
    float AverageDensity;
    float InertiaTensorX;
    float InertiaTensorY;
    float InertiaTensorZ;
    float CollisionOffsetX;
    float CollisionOffsetY;
    float CollisionOffsetZ;
    int DesiredShooterCap;
    Moho::SFootprint Footprint;
    Moho::SFootprint AltFootprint;
    bool LifeBarRender;
    float LifeBarOffset;
    float LifeBarSize;
    float LifeBarHeight;
    float SelectionSizeX;
    float SelectionSizeY;
    float SelectionSizeZ;
    float SelectionCenterOffsetX;
    float SelectionCenterOffsetY;
    float SelectionCenterOffsetZ;
    float SelectionYOffset;
    float SelectionMeshScaleX;
    float SelectionMeshScaleY;
    float SelectionMeshScaleZ;
    float SelectionMeshUseTopAmount;
    float SelectionThickness;
    float UseOOBTestZoom;
    std::string StrategicIconName;
    char StrategicIconSortPriority;
    boost::shared_ptr<unk_t> StrategicIconRest;
    boost::shared_ptr<unk_t> StrategicIconSelected;
    boost::shared_ptr<unk_t> StrategicIconOver;
    boost::shared_ptr<unk_t> StrategicIconSelectedOver;
};

}
