#include "core/Stats.h"
#include "gpgcore/containers/fastvector.h"
#include "boost/shared_ptr.hpp"

namespace Moho {

    
struct SSTIUnitConstantData
{
    bool v0;
    boost::shared_ptr<Moho::Stats<Moho::StatItem>> root;
    bool mFake;
};

struct UnitAttributes
{
    Moho::RUnitBlueprint *mBlueprint;
    int v1;
    Moho::EntityCategory mRestrictionCategory;
    float mElevation;
    float mSpeedMult;
    float mAccMult;
    float mTurnMult;
    float mBreakOffTriggerMult;
    float mBreakOffDistMult;
    float mConsumptionPerSecondEnergy;
    float mConsumptionPerSecondMass;
    float mProductionPerSecondEnergy;
    float mProductionPerSecondMass;
    float mBuildRate;
    float mRegenRate;
    Moho::ERuleBPUnitCommandCaps mCommandCaps;
    Moho::ERuleBPUnitToggleCaps mToggleCaps;
    bool mReclaimable;
    bool mCapturable;
    char gap20A[6];

    UnitAttributes() :
        mBlueprint{nullptr},
        mRestrictionCategory{},
        mElevation{INFINITY},
        mSpeedMult{1.0},
        mAccMult{1.0},
        mTurnMult{1.0},
        mBreakOffTriggerMult{1.0},
        mBreakOffDistMult{1.0},
        mConsumptionPerSecondEnergy{0.0},
        mConsumptionPerSecondMass{0.0},
        mProductionPerSecondEnergy{0.0},
        mProductionPerSecondMass{0.0},
        mBuildRate{1.0},
        mRegenRate{0.0},
        mCommandCaps{0},
        mToggleCaps{0},
        mReclaimable{true},
        mCapturable{true}
    {} // inline 0x005BD8A7
};


struct SSTIUnitVariableData
{
    Moho::TDatListItem<unk_t, void> *aitarget1;
    int mCreationTick;
    bool mAutoMode;
    bool mAutoSurfaceMode;
    bool mBusy;
    float mFuelRatio;
    float mShieldRatio;
    int mStunTicks;
    bool mPaused;
    bool mValidTarget;
    Moho::EJobType mJobType;
    Moho::EFireState mFireState;
    float mWorkProgress;
    int mTacticalSiloBuildCount;
    int mNukeSiloBuildCount;
    int mTacticalSiloStorageCount;
    int mNukeSiloStorageCount;
    int mTacticalSiloMaxStorageCount;
    int mNukeSiloMaxStorageCount;
    Moho::EntId dword40;
    std::string mCustomName;
    Moho::SEconValue mProduced;
    Moho::SEconValue mResourcesSpent;
    Moho::SEconValue mMaintainenceCost;
    Moho::EntId mFocusUnit;
    Moho::EntId dword7C;
    Moho::EntId dword80;
    boost::shared_ptr<Moho::CAniPose> mPriorSharedPose;
    boost::shared_ptr<Moho::RScmResource> mSharedPose;
    int val1;
    gpg::fastvector_n<int, 8> vec1;
    gpg::fastvector_n<int, 4> vec2;
    gpg::fastvector_n<Moho::UnitWeaponInfo, 1> mWeaponInfo;
    Moho::UnitAttributes mAttributes;
    unsigned int mScriptbits;
    int gap;
    Moho::EUnitStateMask mUnitStates;
    bool v4;
    bool mOverchargePaused;
    bool v6;
    bool v7;
};

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

}
