#include "core/blueprints/Blueprint.h"
#include "core/blueprints/Footprint.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

namespace Moho {

enum ECollisionShape
{
    COLSHAPE_None = 0x0,
    COLSHAPE_Box = 0x1,
    COLSHAPE_Sphere = 0x2,
};

class REntityBlueprint : public Moho::RBlueprint
{
public:
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
