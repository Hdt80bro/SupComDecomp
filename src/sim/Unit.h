#include <map>
#include "sim/Entity.h"

namespace Moho {

class Unit;
class UserUnit;

// 0x00E2A514
class IUnit
{
public:
    const Moho::Unit *IsUnit() const { return nullptr; } // 0x006A48C0
    Moho::Unit *IsUnit() { return nullptr; } // 0x006A48B0
    const Moho::UserUnit *IsUserUnit() const { return nullptr; } // 0x006A48E0
    Moho::UserUnit *IsUserUnit() { return nullptr; } // 0x006A48D0
};

struct SSTIUnitConstantData
{
    bool v0;
    boost::shared_ptr<Moho::Stats<Moho::StatItem>> root;
    bool fake;
};

struct UnitAttributes
{
    Moho::RUnitBlueprint *blueprint;
    _DWORD v1;
    Moho::EntityCategory restrictionCategory;
    float elevation;
    float speedMult;
    float accMult;
    float turnMult;
    float breakOffTriggerMult;
    float breakOffDistMult;
    float consumptionPerSecondEnergy;
    float consumptionPerSecondMass;
    float productionPerSecondEnergy;
    float productionPerSecondMass;
    float buildRate;
    float regenRate;
    Moho::ERuleBPUnitCommandCaps commandCaps;
    Moho::ERuleBPUnitToggleCaps toggleCaps;
    bool reclaimable;
    bool capturable;
    char gap20A[6];
};


struct SSTIUnitVariableData
{
    Moho::TDatListItem_1 *aitarget1;
    int creation_tick;
    bool autoMode;
    bool autoSurfaceMode;
    bool isBusy;
    float fuelRatio;
    float shieldRatio;
    int stunTicks;
    bool isPaused;
    bool isValidTarget;
    Moho::EJobType jobType;
    Moho::EFireState fireState;
    float workProgress;
    int tacticalSiloBuildCount;
    int nukeSiloBuildCount;
    int tacticalSiloStorageCount;
    int nukeSiloStorageCount;
    int tacticalSiloMaxStorageCount;
    int nukeSiloMaxStorageCount;
    Moho::EntId dword40;
    std::string customName;
    Moho::SEconValue produced;
    Moho::SEconValue resourcesSpent;
    Moho::SEconValue maintainenceCost;
    Moho::EntId focusUnit;
    Moho::EntId dword7C;
    Moho::EntId dword80;
    boost::shared_ptr<Moho::CAniPose> priorSharedPose;
    boost::shared_ptr<Moho::RScmResource> sharedPose;
    Wm3::Vector3f vec3;
    _DWORD dwordA0;
    _DWORD dwordA4;
    _DWORD gapA8;
    _BYTE v0;
    _BYTE v1;
    _BYTE v2;
    _BYTE v3;
    _BYTE gapB0[24];
    _DWORD dwordC8;
    _DWORD dwordCC;
    _DWORD dwordD0;
    _DWORD dwordD4;
    _DWORD gapD8;
    _DWORD gapDC;
    _DWORD gapE0;
    _DWORD gapE4;
    _DWORD gapE8;
    float gapEC;
    _DWORD gapF0;
    _DWORD gapF4;
    gpg::fastvector<Moho::UnitWeaponInfo> weaponInfo;
    _BYTE gap104[4];
    _BYTE gap108[152];
    Moho::UnitAttributes attributes;
    unsigned int scriptbits;
    _BYTE gap214[4];
    Moho::EUnitStateMask unitStates;
    bool v4;
    bool overchargePaused;
    bool v6;
    bool v7;
};

struct SInfoCache
{
    Moho::CFormationInstance *formationLayer;
    Moho::WeakPtr<Moho::Unit> v1;
    int v2;
    bool v3;
    float topSpeed;
    float v4;
    Wm3::Vector3f v5;
};


class Unit :
    public Moho::IUnit, // 0x00E2A574
    public Moho::Entity // 0x00E2A5EC 0x00E2A688
{
public:
    Moho::SSTIUnitConstantData sstiConst;
    Moho::SSTIUnitVariableData stiVar;
    int v81;
    Moho::CUnitMotion *unitMotion;
    Moho::CUnitCommandQueue *commandQueue;
    Moho::WeakPtr<Moho::Unit> creator;
    Moho::WeakPtr<Moho::Unit> transportedBy;
    Moho::WeakPtr<Moho::Unit> ferryUnit;
    Moho::TDatListItem<Moho::CScriptObject> focusEntity;
    Moho::WeakPtr<Moho::Entity> v92;
    Moho::WeakPtr<Moho::Unit> guardedUnit;
    Wm3::Vector3f v96;
    int v109;
    Moho::EntitySetTemplate<Moho::Unit> v100;
    Moho::CAiFormationInstance *guardFormation;
    bool v111;
    int creationTick;
    Moho::CEconStorage *extraStorage;
    int v114;
    Moho::CEconRequest *consumptionData;
    bool consumptionIsActive;
    bool productionActive;
    bool v116c;
    bool v116d;
    float resourceConsumed;
    Moho::CAniActor *cAniActor;
    Moho::IAiAttacker *attacker;
    Moho::IAiCommandDispatch *commandDispatch;
    Moho::IAiNavigator *navigator;
    Moho::IAiSteering *steering;
    Moho::IAiBuilder *builder;
    Moho::IAiSiloBuild *aiSiloBuild;
    Moho::IAiTransport *storage;
    bool v126;
    float transportLoadFactor;
    std::map<std::string, float> armor;
    int v131;
    int v132;
    char terrainType;
    char debugAIStates;
    Moho::SInfoCache info;
    gpg::Rect2i occupation;
    gpg::fastvector<Moho::WeakPtr<Moho::Entity>> blipsInRange;
    int v151;
    int v152;
    int v153;
    int v154;
    int v155;
    int v156;
    int v157;
    int v158;
    int v159;
    int v160;
    int v161;
    int v162;
    int v163;
    int v164;
    int v165;
    int v166;
    int v167;
    int v168;
    int v169;
    int v170;
    int v171;
    int v172;
    int v173;
    int v174;
    int v175;
    int v176;
    int v177;
    int v178;
    int v179;
    int v180;
    int v181;
    int v182;
    int v183;
    int v184;
    int v185;
    int v186;
    int v187;
    int v188;
    int v189;
    int v190;
    int v191;
    int blipLastUpdateTick;
    int v193;
    gpg::fastvector<Moho::ReconBlip> reconblips;
    int v197;
    int v198;
    int v199;
    bool isnotPod;
    bool isEngineer;
    bool isNaval;
    bool isAir;
    bool usesGridBasedMotion;
    bool isMelee;
    bool hasFocus;
    gpg::RRef v202;
    int v204;
    int v205;
    int v206;
    int v207;
    
};

}
