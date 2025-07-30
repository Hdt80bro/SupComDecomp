#include <string>
#include <vector>
#include "core/reflection/EntityBlueprint.h"
#include "core/MinMax.h"



namespace Moho {

enum ERuleBPUnitCommandCaps
{
    RULEUCC_Move = 0x1,
    RULEUCC_Stop = 0x2,
    RULEUCC_Attack = 0x4,
    RULEUCC_Guard = 0x8,
    RULEUCC_Patrol = 0x10,
    RULEUCC_RetaliateToggle = 0x20,
    RULEUCC_Repair = 0x40,
    RULEUCC_Capture = 0x80,
    RULEUCC_Transport = 0x100,
    RULEUCC_CallTransport = 0x200,
    RULEUCC_Nuke = 0x400,
    RULEUCC_Tactical = 0x800,
    RULEUCC_Teleport = 0x1000,
    RULEUCC_Ferry = 0x2000,
    RULEUCC_SiloBuildTactical = 0x4000,
    RULEUCC_SiloBuildNuke = 0x8000,
    RULEUCC_Sacrifice = 0x10000,
    RULEUCC_Pause = 0x20000,
    RULEUCC_Overcharge = 0x40000,
    RULEUCC_Dive = 0x80000,
    RULEUCC_Reclaim = 0x100000,
    RULEUCC_SpecialAction = 0x200000,
    RULEUCC_Dock = 0x400000,
    RULEUCC_Script = 0x800000,
    RULEUCC_Invalid = 0x1000000,
};

enum ERuleBPUnitToggleCaps
{
    RULEUTC_ShieldToggle = 0x1,
    RULEUTC_WeaponToggle = 0x2,
    RULEUTC_JammingToggle = 0x4,
    RULEUTC_IntelToggle = 0x8,
    RULEUTC_ProductionToggle = 0x10,
    RULEUTC_StealthToggle = 0x20,
    RULEUTC_GenericToggle = 0x40,
    RULEUTC_SpecialToggle = 0x80,
    RULEUTC_CloakToggle = 0x100,
};

enum ERuleBPUnitMovementType
{
    RULEUMT_None = 0x0,
    RULEUMT_Land = 0x1,
    RULEUMT_Air = 0x2,
    RULEUMT_Water = 0x3,
    RULEUMT_Biped = 0x4,
    RULEUMT_SurfacingSub = 0x5,
    RULEUMT_Amphibious = 0x6,
    RULEUMT_Hover = 0x7,
    RULEUMT_AmphibiousFloating = 0x8,
    RULEUMT_Spec = 0x9,
};

enum ERuleBPUnitBuildRestriction
{
    RULEUBR_None = 0x0,
    RULEUBR_Bridge = 0x1,
    RULEUBR_OnMassDeposit = 0x2,
    RULEUBR_OnHydrocarbonDeposit = 0x3,
};

enum ETransportClass
{
    TRANSPORTCLASS_1 = 0x1,
    TRANSPORTCLASS_2 = 0x2,
    TRANSPORTCLASS_3 = 0x3,
    TRANSPORTCLASS_4 = 0x4,
    TRANSPORTCLASS_SPECIAL = 0x5,
};

enum UnitWeaponRangeCategory
{
    UWRC_Undefined = 0x0,
    UWRC_DirectFire = 0x1,
    UWRC_IndirectFire = 0x2,
    UWRC_AntiAir = 0x3,
    UWRC_AntiNavy = 0x4,
    UWRC_Countermeasure = 0x5,
};

enum ERuleBPUnitWeaponBallisticArc
{
    RULEUBA_None = 0x0,
    RULEUBA_LowArc = 0x1,
    RULEUBA_HighArc = 0x2,
};

enum ERuleBPUnitWeaponTargetType
{
    RULEWTT_Unit = 0x0,
    RULEWTT_Projectile = 0x1,
    RULEWTT_Prop = 0x2,
};



struct RaisedPlatform
{
    Wm3::Vector3f p0;
    Wm3::Vector3f p1;
    Wm3::Vector3f p2;
    Wm3::Vector3f p3;
};


struct RUnitBlueprintGeneral
{
    Moho::ERuleBPUnitCommandCaps CommandCaps;
    Moho::ERuleBPUnitToggleCaps ToggleCaps;
    std::string UpgradesTo;
    std::string UpgradesFrom;
    std::string UpgradesFromBase;
    std::string SeedUnit;
    int QuickSelectPriority;
    float CapCost;
    int SelectionPriority;
};


struct RUnitBlueprintDisplay
{
    std::string DisplayName;
    std::string MeshBlueprint;
    std::string PlaceholderMeshName;
    std::string IconName;
    float UniformScale;
    bool SpawnRandomRotation;
    bool HideLifebars;
};

struct RUnitBlueprintPhysics
{
    bool FlattenSkirt;
    float SkirtOffsetX;
    float SkirtOffsetZ;
    float SkirtSizeX;
    float SkirtSizeZ;
    float MaxGroundVariation;
    Moho::ERuleBPUnitMovementType MotionType;
    Moho::ERuleBPUnitMovementType AltMotionType;
    bool StandUpright;
    bool SinkLower;
    bool RotateBodyWhileMoving;
    float DiveSurfaceSpeed;
    float MaxSpeed;
    float MaxSpeedReverse;
    float MaxAcceleration;
    float MaxBrake;
    float MaxSteerForce;
    float BankingSlope;
    float RollStability;
    float RollDamping;
    float WobbleFactor;
    float WobbleSpeed;
    float TurnRadius;
    float TurnRate;
    float TurnFacingRate;
    bool RotateOnSpot;
    float RotateOnSpotThreshold;
    float Elevation;
    float AttackElevation;
    float CatchUpAcc;
    float BackUpDistance;
    float LayerChangeOffsetHeight;
    float LayerTransitionDuration;
    Moho::ELayer BuildOnLayerCaps;
    Moho::ERuleBPUnitBuildRestriction BuildRestriction;
    Moho::SFootprint *footprint;
    Moho::SFootprint *altFootprint;
    float FuelUseTime;
    float FuelRechargeRate;
    float GroundCollisionOffset;
    std::vector<Moho::RaisedPlatform> OccupRects;
    std::vector<Moho::RaisedPlatform> RaisedPlatforms;
};


struct RUnitBlueprintIntel
{
    unsigned int VisionRadius;
    unsigned int WaterVisionRadius;
    unsigned int RadarRadius;
    unsigned int SonarRadius;
    unsigned int OmniRadius;
    bool RadarStealth;
    bool SonarStealth;
    bool Cloak;
    bool ShowIntelOnSelect;
    unsigned int RadarStealthFieldRadius;
    unsigned int SonarStealthFieldRadius;
    unsigned int CloakFieldRadius;
    Moho::SMinMax<unsigned int> JamRadius;
    char JammerBlips;
    Moho::SMinMax<unsigned int> SpoofRadius;
};


struct RUnitBlueprintAir
{
    bool CanFly;
    bool Winged;
    bool FlyInWater;
    float AutoLandTime;
    float MaxAirSpeed;
    float MinAirSpeed;
    float TurnSpeed;
    float CombatTurnSpeed;
    float StartTurnDistance;
    float TightTurnMultiplier;
    float SustainedTurnThreshold;
    float LiftFactor;
    float BankFactor;
    bool BankForward;
    float EngageDistance;
    float BreakOffTriggger;
    float BreakOffDistance;
    bool BreakOffIfNearNewTarget;
    float KMove;
    float KMoveDamping;
    float KLift;
    float KLiftDamping;
    float KTurn;
    float KTurnDamping;
    float KRoll;
    float KRollDamping;
    float CirclingTurnMult;
    float CirclingRadiusChangeMinRatio;
    float CirclingRadiusChangeMaxRatio;
    float CirclingRadiusVsAirMult;
    float CirclingElevationChangeRatio;
    float CirclingFlightChangeFrequency;
    bool CirclingDirChange;
    bool HoverOverAttack;
    float RandomBreakOffDistanceMult;
    float RandomMinChangeCombatStateTime;
    float RandomMaxChangeCombatStateTime;
    float TransportHoverHeight;
    float PredictAheadForBombDrop;
};

struct RUnitBlueprintTransport
{
    Moho::ETransportClass TransportClass;
    int ClassGenericUpTo;
    int Class2AttachSize;
    int Class3AttachSize;
    int Class4AttachSize;
    int ClassSAttachSize;
    bool AirClass;
    int StorageSlots;
    int DockingSlots;
    float RepairsRate;
};


struct RUnitBlueprintDefenseShield
{
    float ShieldSize;
    float RegenAssistMult;
};


struct RUnitBlueprintDefense
{
    float MaxHealth;
    float Health;
    float RegenRate;
    float AirThreatLevel;
    float SurfaceThreatLevel;
    float SubThreatLevel;
    float EconomyThreatLevel;
    std::string ArmorType;
    Moho::RUnitBlueprintDefenseShield Shield;
};


struct RUnitBlueprintAI
{
    float GuardScanRadius;
    float GuardReturnRadius;
    float StagingPlatformScanRadius;
    bool ShowAssistRangeOnSelect;
    std::string GuardFormationName;
    bool NeedUnpack;
    bool InitialAutoMode;
    std::string BeaconName;
    std::vector<int> TargetBones;
    float RefuelingMultiplier;
    float RefuelingRepairAmount;
    float RepairConsumeEnergy;
    float RepairConsumeMass;
    bool AutoSurfaceToAttack;
    float AttackAngle;
};

struct RUnitBlueprintWeapon
{
    int v1;
    int v2;
    std::string Label;
    std::string DisplayName;
    Moho::UnitWeaponRangeCategory RangeCategory;
    bool DummyWeapon;
    bool PrefersPrimaryWeaponTarget;
    bool StopOnPrimaryWeaponBusy;
    bool SlavedToBody;
    float SlavedToBodyArcRange;
    bool AutoInitiateAttackCommand;
    float TargetCheckInterval;
    bool AlwaysRecheckTarget;
    float MinRadius;
    float MaxRadius;
    float MaximumBeamLength;
    float EffectiveRadius;
    float MaxHeightDiff;
    float TrackingRadius;
    float HeadingArcCenter;
    float HeadingArcRange;
    float FiringTolerance;
    float FiringRandomness;
    float RequiresEnergy;
    float RequiresMass;
    float MuzzleVelocity;
    float MuzzleVelocityRandom;
    float MuzzleVelocityReduceDistance;
    bool LeadTarget;
    float ProjectileLifetime;
    float ProjectileLifetimeUsesMultiplier;
    float Damage;
    float DamageRadius;
    std::string DamageType;
    float RateOfFire;
    std::string ProjectileId;
    Moho::ERuleBPUnitWeaponBallisticArc BallisticArc;
    std::string TargetRestrictOnlyAllow;
    std::string TargetRestrictOnlyDisallow;
    bool ManualFire;
    bool NukeWeapon;
    bool OverChargeWeapon;
    bool NeedPrep;
    bool CountedProjectile;
    int MaxProjectileStorage;
    bool IgnoresAlly;
    Moho::ERuleBPUnitWeaponTargetType TargetType;
    int AttackGroundTries;
    bool AimsStraightOnDisable;
    bool Turreted;
    bool YawOnlyOnTarget;
    bool AboveWaterFireOnly;
    bool BelowWaterFireOnly;
    bool AboveWaterTargetsOnly;
    bool BelowWaterTargetsonly;
    bool ReTargetOnMiss;
    bool NeedToComputeBombDrop;
    float BombDropThreshold;
    bool UseFiringSolutionInsteadOfAimBone;
    bool IgnoreIfDisabled;
    bool CannotAttackGround;
    std::string UIMinRangeVisualId;
    std::string UIMaxRangeVisualId;
};


struct RUnitBlueprintEconomy
{
    float BuildCostEnergy;
    float BuildCostMass;
    float BuildRate;
    float BuildTime;
    float StorageEnergy;
    float StorageMass;
    bool NaturalProducer;
    std::vector<std::string> BuildableCategories;
    std::vector<std::string> RebuildBonusIds;
    int v1;
    Moho::EntityCategory cat;
    float InitialRallyX;
    float InitialRallyZ;
    bool NeedToFaceTargetToBuild;
    float SacrificeMassMult;
    float SacrificeEnergyMult;
    float MaxBuildDistance;
};


// 0x00E15638
class RUnitBlueprint : public Moho::REntityBlueprint
{
public:
    Moho::RUnitBlueprintGeneral General;
    Moho::RUnitBlueprintDisplay Display;
    Moho::RUnitBlueprintPhysics Physics;
    Moho::RUnitBlueprintIntel Intel;
    Moho::RUnitBlueprintAir Air;
    Moho::RUnitBlueprintTransport Transport;
    Moho::RUnitBlueprintDefense Defense;
    Moho::RUnitBlueprintAI AI;
    std::vector<Moho::RUnitBlueprintWeapon> Weapons;
    int gap;
    Moho::RUnitBlueprintEconomy Economy;
};

}

