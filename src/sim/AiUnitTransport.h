#include <vector>

namespace Moho {

struct SAiReservedTransportBone
{
    int v1;
    int v2;
    Moho::WeakPtr<Moho::Unit> unit;
    std::vector<int> bones;
};

struct SAttachPoint
{
    int index;
    Wm3::Vector3f localPos;
    float distSq;
};

struct STransportPickUpInfo
{
    Moho::SCoordsVec2 v0;
    Wm3::Quaternion ori;
    Wm3::Vector3f pos;
    int v1;
    Moho::EntitySetTemplate<Moho::Unit> units;
    bool hasSpace;
    int v67;
};

class CAiTransportImpl
{
    int v1;
    int v2;
    Moho::Unit *unit;
    Moho::WeakPtr<Moho::Unit> transportUnit;
    bool stagingPlatform;
    bool teleportation;
    bool v6c;
    bool v6d;
    int attachpoints;
    int nextGeneric;
    int v9;
    int genericOverflow;
    int v11;
    Moho::EntitySetTemplate<Moho::Unit> v12;
    Moho::EntitySetTemplate<Moho::Unit> v22;
    Moho::EntitySetTemplate<Moho::Unit> v32;
    std::vector<Moho::SAiReservedTransportBone> v42;
    Moho::STransportPickUpInfo res;
    Moho::CAiFormationInstance *formationInstance;
    Wm3::Vector3f v68;
    std::vector<Moho::SAttachPoint> genericAttachPoints;
    std::vector<Moho::SAttachPoint> class1AttachPoints;
    std::vector<Moho::SAttachPoint> class2AttachPoints;
    std::vector<Moho::SAttachPoint> class3AttachPoints;
    std::vector<Moho::SAttachPoint> class4AttachPoints;
    std::vector<Moho::SAttachPoint> classSAttachPoints;
    std::vector<Moho::SAttachPoint> launchPoints;
    int v99;
};

}
