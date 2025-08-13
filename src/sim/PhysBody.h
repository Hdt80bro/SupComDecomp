#include "Wm3Vector3.h"
#include "Wm3Quaternion.h"

namespace Moho {

struct SPhysBody
{
    float mGravity;
    float mMass;
    Wm3::Vector3f mInvInertiaTensor;
    Wm3::Vector3f mCollisionOffset;
    Wm3::Vector3f mPos;
    Wm3::Quaternionf mOrientation;
    Wm3::Vector3f mVelocity;
    Wm3::Vector3f mWorldImpulse;
};


}
