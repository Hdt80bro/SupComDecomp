#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "LuaPlus.h"
#include "Wm3AxisAlignedBox3.h"
#include "Wm3Quaternion.h"
#include "Wm3Vector3.h"
#include "core/Intel.h"
#include "core/ScriptObject.h"
#include "core/SndParams.h"
#include "core/Task.h"
#include "sim/ColPrimitive.h"
#include "sim/PhysBody.h"
#include "sim/Sim.h"

namespace Moho {

enum ELayer
{
    LAYER_None = 0x0,
    LAYER_Land = 0x1,
    LAYER_Seabed = 0x2,
    LAYER_Sub = 0x4,
    LAYER_Water = 0x8,
    LAYER_Air = 0x10,
    LAYER_Orbit = 0x20,
    LAYER_All = 0x7F,
};


enum EEntityType
{
    ENTITYTYPE_Unit = 0x100,
    ENTITYTYPE_Prop = 0x200,
    ENTITYTYPE_Projectile = 0x400,
    ENTITYTYPE_Entity = 0x800,
};

enum EVisibilityMode
{
    VIZMODE_Never = 0x1,
    VIZMODE_Always = 0x2,
    VIZMODE_Intel = 0x4,
};


using EntId = int;

struct SEntAttachInfo
{
    int *v0;
    int v1;
    float v2;
    float v3;
    Wm3::Quaternionf mParentOrientation;
    Wm3::Vector3f mParentOffset;
};


struct SSTIEntityConstantData
{
    Moho::EntId mId;
    Moho::REntityBlueprint *mBlueprint;
    int mTickCreated;
};

struct EntityAttributes
{
    int mVision;
    int mWaterVision;
    int mRadar;
    int mSonar;
    int mOmni;
    int mRadarStealth;
    int mSonarStealth;
    int mCloak;
};


struct SSTIEntityVariableData
{
    boost::shared_ptr<Moho::RScmResource> mScmResource;
    int mMesh;
    Wm3::Vector3f mScale;
    float mHealth;
    float mMaxHealth;
    bool mIsBeingBuilt;
    bool mIsDead;
    bool mRequestRefreshUI;
    Moho::VTransform mCurTransform;
    Moho::VTransform mLastTransform;
    float mCurImpactSomething;
    float mFractionComplete;
    int mAttachmentParent;
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    long v7;
    long v8;
    Moho::CSndParams *mAmbientSound;
    Moho::CSndParams *mRumbleSound;
    bool mNotVisibility;
    int mVisibility;
    Moho::ELayer mLayer;
    bool mUsingAltFP;
    boost::shared_ptr<Moho::CD3DBatchTexture> mUnderlayTexture;
    Moho::EntityAttributes mAttributes;
};



class Entity :
    public Moho::CScriptObject, // 0x00E274F4
    public Moho::CTask          // 0x00E27590
{
public:
    Moho::EEntityType mType;
    Moho::TDatListItem<unk_t> mTarget;
    Moho::SSTIEntityConstantData mConstDat;
    int gap;
    Moho::SSTIEntityVariableData mVarDat;
    Moho::Sim *mSim;
    Moho::IArmy *mArmy;
    Moho::VTransform mLastTrans;
    Moho::PositionHistory *mPositionHistory;
    float mLastImpactSomething;
    int v6;
    Moho::CColPrimitive *mCollisionShape;
    std::vector<Moho::Entity *> mAttachedEntities;
    Moho::SEntAttachInfo parent_p1;
    bool v3a;
    bool mDestroyQueued;
    bool mDestroyed;
    std::string mResId;
    Moho::CIntel *mIntelManager;
    Moho::EVisibilityMode mVizToFocusPlayer;
    Moho::EVisibilityMode mVizToAllies;
    Moho::EVisibilityMode mVizToEnemies;
    Moho::EVisibilityMode mVizToNeutrals;
    bool mInterfaceCreated;
    Moho::CTextureScroller *mScroller;
    Moho::SPhysBody *mPhysBody;
    bool v4;
    std::string mUniqueName;
    Moho::EntitySetTemplate<Moho::Entity> mShooters;
    Wm3::AxisAlignedBox3f mCollision;
    LuaPlus::LuaObject mObjPos;
};

int ENTSCR_ResolveBoneIndex(Moho::Entity *, LuaPlus::LuaStackObject &, bool); // 0x00692580
Wm3::Vector3f ENTSCR_GetBonePosition(Moho::Entity *, LuaPlus::LuaStackObject &, bool); // 0x006926C0

}
