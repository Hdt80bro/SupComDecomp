#include "core/reflection/EntityBlueprint.h"
#include <string>


namespace Moho {

struct RProjectileBlueprintDisplay
{
    std::string mMeshBlueprint;
    float mUniformScale;
    float mMeshScaleRange;
    float mMeshScaleVelocity;
    float mMeshScaleVelocityRange;
    bool mCameraFollowsProjectile;
    float mCameraFollowTimeout;
    float mStrategicIconSize;
};

struct RProjectileBlueprintEconomy
{
    float mBuildCostEnergy;
    float mBuildCostMass;
    float mBuildTime;
};

struct RProjectileBlueprintPhysics
{
    bool mCollideSurface;
    bool mCollisionEntity;
    bool mTrackTarget;
    bool mVelocityAlign;
    bool mStayUpright;
    bool mLeadTarget;
    bool mStayUnderwater;
    bool mUseGravity;
    float mDetonateAboveHeight;
    float mDetonateBelowHeight;
    float mTurnRate;
    float mTurnRateRange;
    float mLifetime;
    float mLifeTimeRange;
    float mInitialSpeed;
    float mInitialSpeedRange;
    float mMaxSpeed;
    float mMaxSpeedRange;
    float mAcceleration;
    float mAccelerationRange;
    float mPositionX;
    float mPositionY;
    float mPositionZ;
    float mPositionXRange;
    float mPositionYRange;
    float mPositionZRange;
    float mDirectionX;
    float mDirectionY;
    float mDirectionZ;
    float mDirectionXRange;
    float mDirectionYRange;
    float mDirectionZRange;
    float mRotationalVelocity;
    float mRotationalVelocityRange;
    float mMaxZigZag;
    float mZigZagFrequency;
    bool mDestroyOnWater;
    int mMinBounceCount;
    int mMaxBounceCount;
    float mBounceVelDamp;
    bool mRealisticOrdinance;
    bool mStraightDownOrdinance;
};

// 0x00E10CF0
class RProjectileBlueprint : public Moho::REntityBlueprint
{
public:
    std::string mDevStatus;
    Moho::RProjectileBlueprintDisplay mDisplay;
    Moho::RProjectileBlueprintEconomy mEconomy;
    Moho::RProjectileBlueprintPhysics mPhysics;
};


}
