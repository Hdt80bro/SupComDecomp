#include "core/reflection/EntityBlueprint.h"
#include "core/MinMax.h"
#include <string>
#include <vector>


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
    Moho::ERuleBPUnitCommandCaps mCommandCaps;
    Moho::ERuleBPUnitToggleCaps mToggleCaps;
    std::string mUpgradesTo;
    std::string mUpgradesFrom;
    std::string mUpgradesFromBase;
    std::string mSeedUnit;
    int mQuickSelectPriority;
    float mCapCost;
    int mSelectionPriority;
};

struct RUnitBlueprintDisplay
{
    std::string mDisplayName;
    std::string mMeshBlueprint;
    std::string mPlaceholderMeshName;
    std::string mIconName;
    float mUniformScale;
    bool mSpawnRandomRotation;
    bool mHideLifebars;
};

struct RUnitBlueprintPhysics
{
    bool mFlattenSkirt;
    float mSkirtOffsetX;
    float mSkirtOffsetZ;
    float mSkirtSizeX;
    float mSkirtSizeZ;
    float mMaxGroundVariation;
    Moho::ERuleBPUnitMovementType mMotionType;
    Moho::ERuleBPUnitMovementType mAltMotionType;
    bool mStandUpright;
    bool mSinkLower;
    bool mRotateBodyWhileMoving;
    float mDiveSurfaceSpeed;
    float mMaxSpeed;
    float mMaxSpeedReverse;
    float mMaxAcceleration;
    float mMaxBrake;
    float mMaxSteerForce;
    float mBankingSlope;
    float mRollStability;
    float mRollDamping;
    float mWobbleFactor;
    float mWobbleSpeed;
    float mTurnRadius;
    float mTurnRate;
    float mTurnFacingRate;
    bool mRotateOnSpot;
    float mRotateOnSpotThreshold;
    float mElevation;
    float mAttackElevation;
    float mCatchUpAcc;
    float mBackUpDistance;
    float mLayerChangeOffsetHeight;
    float mLayerTransitionDuration;
    Moho::ELayer mBuildOnLayerCaps;
    Moho::ERuleBPUnitBuildRestriction mBuildRestriction;
    Moho::SFootprint *mFootprint;
    Moho::SFootprint *mAltFootprint;
    float mFuelUseTime;
    float mFuelRechargeRate;
    float mGroundCollisionOffset;
    std::vector<Moho::RaisedPlatform> mOccupRects;
    std::vector<Moho::RaisedPlatform> mRaisedPlatforms;
};

struct RUnitBlueprintIntel
{
    unsigned int mVisionRadius;
    unsigned int mWaterVisionRadius;
    unsigned int mRadarRadius;
    unsigned int mSonarRadius;
    unsigned int mOmniRadius;
    bool mRadarStealth;
    bool mSonarStealth;
    bool mCloak;
    bool mShowIntelOnSelect;
    unsigned int mRadarStealthFieldRadius;
    unsigned int mSonarStealthFieldRadius;
    unsigned int mCloakFieldRadius;
    Moho::SMinMax<unsigned int> mJamRadius;
    char mJammerBlips;
    Moho::SMinMax<unsigned int> mSpoofRadius;
};

struct RUnitBlueprintAir
{
    bool mCanFly;
    bool mWinged;
    bool mFlyInWater;
    float mAutoLandTime;
    float mMaxAirSpeed;
    float mMinAirSpeed;
    float mTurnSpeed;
    float mCombatTurnSpeed;
    float mStartTurnDistance;
    float mTightTurnMultiplier;
    float mSustainedTurnThreshold;
    float mLiftFactor;
    float mBankFactor;
    bool mBankForward;
    float mEngageDistance;
    float mBreakOffTriggger;
    float mBreakOffDistance;
    bool mBreakOffIfNearNewTarget;
    float mKMove;
    float mKMoveDamping;
    float mKLift;
    float mKLiftDamping;
    float mKTurn;
    float mKTurnDamping;
    float mKRoll;
    float mKRollDamping;
    float mCirclingTurnMult;
    float mCirclingRadiusChangeMinRatio;
    float mCirclingRadiusChangeMaxRatio;
    float mCirclingRadiusVsAirMult;
    float mCirclingElevationChangeRatio;
    float mCirclingFlightChangeFrequency;
    bool mCirclingDirChange;
    bool mHoverOverAttack;
    float mRandomBreakOffDistanceMult;
    float mRandomMinChangeCombatStateTime;
    float mRandomMaxChangeCombatStateTime;
    float mTransportHoverHeight;
    float mPredictAheadForBombDrop;
};

struct RUnitBlueprintTransport
{
    Moho::ETransportClass mTransportClass;
    int mClassGenericUpTo;
    int mClass2AttachSize;
    int mClass3AttachSize;
    int mClass4AttachSize;
    int mClassSAttachSize;
    bool mAirClass;
    int mStorageSlots;
    int mDockingSlots;
    float mRepairsRate;
};

struct RUnitBlueprintDefenseShield
{
    float mShieldSize;
    float mRegenAssistMult;
};

struct RUnitBlueprintDefense
{
    float mMaxHealth;
    float mHealth;
    float mRegenRate;
    float mAirThreatLevel;
    float mSurfaceThreatLevel;
    float mSubThreatLevel;
    float mEconomyThreatLevel;
    std::string mArmorType;
    Moho::RUnitBlueprintDefenseShield mShield;
};

struct RUnitBlueprintAI
{
    float mGuardScanRadius;
    float mGuardReturnRadius;
    float mStagingPlatformScanRadius;
    bool mShowAssistRangeOnSelect;
    std::string mGuardFormationName;
    bool mNeedUnpack;
    bool mInitialAutoMode;
    std::string mBeaconName;
    std::vector<int> mTargetBones;
    float mRefuelingMultiplier;
    float mRefuelingRepairAmount;
    float mRepairConsumeEnergy;
    float mRepairConsumeMass;
    bool mAutoSurfaceToAttack;
    float mAttackAngle;
};

struct RUnitBlueprintWeapon
{
    int v1;
    int v2;
    std::string mLabel;
    std::string mDisplayName;
    Moho::UnitWeaponRangeCategory mRangeCategory;
    bool mDummyWeapon;
    bool mPrefersPrimaryWeaponTarget;
    bool mStopOnPrimaryWeaponBusy;
    bool mSlavedToBody;
    float mSlavedToBodyArcRange;
    bool mAutoInitiateAttackCommand;
    float mTargetCheckInterval;
    bool mAlwaysRecheckTarget;
    float mMinRadius;
    float mMaxRadius;
    float mMaximumBeamLength;
    float mEffectiveRadius;
    float mMaxHeightDiff;
    float mTrackingRadius;
    float mHeadingArcCenter;
    float mHeadingArcRange;
    float mFiringTolerance;
    float mFiringRandomness;
    float mRequiresEnergy;
    float mRequiresMass;
    float mMuzzleVelocity;
    float mMuzzleVelocityRandom;
    float mMuzzleVelocityReduceDistance;
    bool mLeadTarget;
    float mProjectileLifetime;
    float mProjectileLifetimeUsesMultiplier;
    float mDamage;
    float mDamageRadius;
    std::string mDamageType;
    float mRateOfFire;
    std::string mProjectileId;
    Moho::ERuleBPUnitWeaponBallisticArc mBallisticArc;
    std::string mTargetRestrictOnlyAllow;
    std::string mTargetRestrictOnlyDisallow;
    bool mManualFire;
    bool mNukeWeapon;
    bool mOverChargeWeapon;
    bool mNeedPrep;
    bool mCountedProjectile;
    int mMaxProjectileStorage;
    bool mIgnoresAlly;
    Moho::ERuleBPUnitWeaponTargetType mTargetType;
    int mAttackGroundTries;
    bool mAimsStraightOnDisable;
    bool mTurreted;
    bool mYawOnlyOnTarget;
    bool mAboveWaterFireOnly;
    bool mBelowWaterFireOnly;
    bool mAboveWaterTargetsOnly;
    bool mBelowWaterTargetsonly;
    bool mReTargetOnMiss;
    bool mNeedToComputeBombDrop;
    float mBombDropThreshold;
    bool mUseFiringSolutionInsteadOfAimBone;
    bool mIgnoreIfDisabled;
    bool mCannotAttackGround;
    std::string mUIMinRangeVisualId;
    std::string mUIMaxRangeVisualId;
};

struct RUnitBlueprintEconomy
{
    float mBuildCostEnergy;
    float mBuildCostMass;
    float mBuildRate;
    float mBuildTime;
    float mStorageEnergy;
    float mStorageMass;
    bool mNaturalProducer;
    std::vector<std::string> mBuildableCategories;
    std::vector<std::string> mRebuildBonusIds;
    int v1;
    Moho::EntityCategory cat;
    float mInitialRallyX;
    float mInitialRallyZ;
    bool mNeedToFaceTargetToBuild;
    float mSacrificeMassMult;
    float mSacrificeEnergyMult;
    float mMaxBuildDistance;
};

// 0x00E15638
class RUnitBlueprint : public Moho::REntityBlueprint
{
public:
    Moho::RUnitBlueprintGeneral mGeneral;
    Moho::RUnitBlueprintDisplay mDisplay;
    Moho::RUnitBlueprintPhysics mPhysics;
    Moho::RUnitBlueprintIntel mIntel;
    Moho::RUnitBlueprintAir mAir;
    Moho::RUnitBlueprintTransport mTransport;
    Moho::RUnitBlueprintDefense mDefense;
    Moho::RUnitBlueprintAI mAI;
    std::vector<Moho::RUnitBlueprintWeapon> mWeapons;
    int gap;
    Moho::RUnitBlueprintEconomy mEconomy;
};

}

