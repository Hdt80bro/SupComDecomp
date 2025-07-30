#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "LuaPlus.h"
#include "Wm3AxisAlignedBox3.h"
#include "Wm3Quaternion.h"
#include "Wm3Vector3.h"
#include "core/ScriptObject.h"
#include "core/SndParams.h"
#include "core/Task.h"
#include "sim/ColPrimitive.h"
#include "sim/Intel.h"
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
    Wm3::Quaternionf parentOrientation;
    Wm3::Vector3f parentOffset;
};


struct SSTIEntityConstantData
{
    Moho::EntId id;
    Moho::REntityBlueprint *bp;
    int tickCreated;
};

struct EntityAttributes
{
    int vision;
    int waterVision;
    int radar;
    int sonar;
    int omni;
    int radarStealth;
    int sonarStealth;
    int cloak;
};


struct SSTIEntityVariableData
{
    boost::shared_ptr<Moho::RScmResource> scmResource;
    int mesh;
    Wm3::Vector3f scale;
    float health;
    float maxHealth;
    bool isBeingBuilt;
    bool isDead;
    bool requestRefreshUI;
    Moho::VTransform curTransform;
    Moho::VTransform lastTransform;
    float curImpactSomething;
    float fractionComplete;
    int attachmentParent;
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    long v7;
    long v8;
    Moho::CSndParams *ambientSound;
    Moho::CSndParams *rumbleSound;
    bool notVisibility;
    int visibility;
    Moho::ELayer layer;
    bool usingAltFp;
    boost::shared_ptr<Moho::CD3DBatchTexture> underlayTexture;
    Moho::EntityAttributes intelAttributes;
};



class Entity :
    public Moho::CScriptObject, // 0x00E274F4
    public Moho::CTask          // 0x00E27590
{
public:
    Moho::EEntityType type;
    Moho::TDatListItem<unk_t> aitarget;
    Moho::SSTIEntityConstantData sstiConst;
    int gap;
    Moho::SSTIEntityVariableData sstiVar;
    Moho::Sim *sim;
    Moho::CArmyImpl *army;
    Moho::VTransform lastTrans;
    Moho::PositionHistory *positionHistory;
    float lastImpactSomething;
    int v6;
    Moho::CColPrimitive *collisionShape;
    std::vector<Moho::Entity *> attachedEntities;
    Moho::SEntAttachInfo parent_p1;
    bool v3a;
    bool destroyQueued;
    bool destroyed;
    std::string resId;
    Moho::CIntel *intelManager;
    Moho::EVisibilityMode vizToFocusPlayer;
    Moho::EVisibilityMode vizToAllies;
    Moho::EVisibilityMode vizToEnemies;
    Moho::EVisibilityMode vizToNeutrals;
    bool interfaceCreated;
    Moho::CTextureScroller *scroller;
    Moho::SPhysBody *physBody;
    bool v4;
    std::string uniqueName;
    Moho::EntitySetTemplate<Moho::Entity> shooters;
    Wm3::AxisAlignedBox3f collision;
    LuaPlus::LuaObject objPos;
};

int ENTSCR_ResolveBoneIndex(Moho::Entity *, LuaPlus::LuaStackObject &, bool); // 0x00692580
Wm3::Vector3f ENTSCR_GetBonePosition(Moho::Entity *, LuaPlus::LuaStackObject &, bool); // 0x006926C0

}
