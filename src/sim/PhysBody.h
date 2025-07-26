#include "Wm3Vector3.h"
#include "Wm3Quaternion.h"

namespace Moho {

struct SPhysBody
{
    float gravity;
    float mass;
    Wm3::Vector3f invInertiaTensor;
    Wm3::Vector3f collisionOffset;
    Wm3::Vector3f pos;
    Wm3::Quaternionf orientation;
    Wm3::Vector3f velocity;
    Wm3::Vector3f worldImpulse;
};


}
