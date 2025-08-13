#include <vector>
#include "Wm3Vector3.h"
#include "core/TDatListItem.h"

namespace Moho {

struct SAiReservedTransportBone
{
    int v1;
    int v2;
    Moho::WeakPtr<Moho::Unit> mUnit;
    std::vector<int> mBones;
};

struct SAttachPoint
{
    int index;
    Wm3::Vector3f mLocalPos;
    float mDistSq;
};

struct STransportPickUpInfo
{
    Moho::SCoordsVec2 v0;
    Wm3::Quaternionf mOri;
    Wm3::Vector3f mPos;
    int v1;
    Moho::EntitySetTemplate<Moho::Unit> mUnits;
    bool mHasSpace;
    int v67;
};

// 0x00E1F0AC
class IAiTransport
{

};

// 0x00E1F3CC
class CAiTransportImpl :
    public Moho::IAiTransport,
    public Moho::TDatListItem<Moho::CAiTransportImpl>
{
public:
    Moho::Unit *mUnit;
    Moho::WeakPtr<Moho::Unit> mTransportUnit;
    bool mStagingPlatform;
    bool mTeleportation;
    bool v6c;
    bool v6d;
    int mAttachpoints;
    int mNextGeneric;
    int v9;
    int mGenericOverflow;
    int v11;
    Moho::EntitySetTemplate<Moho::Unit> v12;
    Moho::EntitySetTemplate<Moho::Unit> v22;
    Moho::EntitySetTemplate<Moho::Unit> v32;
    std::vector<Moho::SAiReservedTransportBone> v42;
    Moho::STransportPickUpInfo res;
    Moho::CAiFormationInstance *mFormationInstance;
    Wm3::Vector3f v68;
    std::vector<Moho::SAttachPoint> mGenericAttachPoints;
    std::vector<Moho::SAttachPoint> mClass1AttachPoints;
    std::vector<Moho::SAttachPoint> mClass2AttachPoints;
    std::vector<Moho::SAttachPoint> mClass3AttachPoints;
    std::vector<Moho::SAttachPoint> mClass4AttachPoints;
    std::vector<Moho::SAttachPoint> mClassSAttachPoints;
    std::vector<Moho::SAttachPoint> mLaunchPoints;
    int v99;
};

}
