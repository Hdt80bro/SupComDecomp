#include <string>
#include "core/reflection/EntityBlueprint.h"


namespace Moho {

struct RProjectileBlueprintDisplay
{
    std::string MeshBlueprint;
    float UniformScale;
    float MeshScaleRange;
    float MeshScaleVelocity;
    float MeshScaleVelocityRange;
    bool CameraFollowsProjectile;
    float CameraFollowTimeout;
    float StrategicIconSize;
};


struct RProjectileBlueprintEconomy
{
    float BuildCostEnergy;
    float BuildCostMass;
    float BuildTime;
};

struct RProjectileBlueprintPhysics
{
    bool CollideSurface;
    bool CollisionEntity;
    bool TrackTarget;
    bool VelocityAlign;
    bool StayUpright;
    bool LeadTarget;
    bool StayUnderwater;
    bool UseGravity;
    float DetonateAboveHeight;
    float DetonateBelowHeight;
    float TurnRate;
    float TurnRateRange;
    float Lifetime;
    float LifeTimeRange;
    float InitialSpeed;
    float InitialSpeedRange;
    float MaxSpeed;
    float MaxSpeedRange;
    float Acceleration;
    float AccelerationRange;
    float PositionX;
    float PositionY;
    float PositionZ;
    float PositionXRange;
    float PositionYRange;
    float PositionZRange;
    float DirectionX;
    float DirectionY;
    float DirectionZ;
    float DirectionXRange;
    float DirectionYRange;
    float DirectionZRange;
    float RotationalVelocity;
    float RotationalVelocityRange;
    float MaxZigZag;
    float ZigZagFrequency;
    bool DestroyOnWater;
    int MinBounceCount;
    int MaxBounceCount;
    float BounceVelDamp;
    bool RealisticOrdinance;
    bool StraightDownOrdinance;
};

// 0x00E10CF0
class RProjectileBlueprint : public Moho::REntityBlueprint
{
public:
    std::string DevStatus;
    Moho::RProjectileBlueprintDisplay Display;
    Moho::RProjectileBlueprintEconomy Economy;
    Moho::RProjectileBlueprintPhysics Physics;
};


}
