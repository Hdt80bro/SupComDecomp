#include "Wm3Box3.h"
#include "Wm3Vector3.h"
#include "Wm3Sphere3.h"

struct struct_CollisionEntry
{
    int unk;
    Wm3::Vector3f dir;
    Wm3::Vector3f pos;
    float dist;
};


namespace Moho {

// 0x00E0D3F4
class CColPrimitiveBase
{
public:
    virtual Wm3::AxisAlignedBox3f GetBoundingBox() = 0;
    virtual Wm3::Sphere3f *GetSphere() = 0;
    virtual Wm3::Box3f *GetBox() = 0;
    virtual void SetTransform(Moho::VTransform *) = 0;
    virtual Wm3::Vector3f GetCenter() = 0;
    virtual void SetCenter(Wm3::Vector3f *) = 0;
    virtual bool CollideLine(Wm3::Vector3f *p1, Wm3::Vector3f *p2, struct_CollisionEntry *out) = 0;
    virtual bool CollideBox(Wm3::Box3f *box, Moho::CollisionResult *res) = 0;
    virtual bool CollideSphere(Wm3::Sphere3f *box, Moho::CollisionResult *res) = 0;
    virtual bool PointInShape(Wm3::Vector3f *p) = 0;
};


template <class T>
class CColPrimitive
{
    T shape;
    Wm3::Vector3f center;

    virtual Wm3::AxisAlignedBox3f GetBoundingBox() = 0;
    virtual Wm3::Sphere3f *GetSphere() = 0;
    virtual Wm3::Box3f *GetBox() = 0;
    virtual void SetTransform(Moho::VTransform *) = 0;
    virtual Wm3::Vector3f GetCenter() = 0;
    virtual void SetCenter(Wm3::Vector3f *) = 0;
    virtual bool CollideLine(Wm3::Vector3f *p1, Wm3::Vector3f *p2, struct_CollisionEntry *out) = 0;
    virtual bool CollideBox(Wm3::Box3f *box, Moho::CollisionResult *res) = 0;
    virtual bool CollideSphere(Wm3::Sphere3f *box, Moho::CollisionResult *res) = 0;
    virtual bool PointInShape(Wm3::Vector3f *p) = 0;
};


// 0x00E0D50C
template<>
class CColPrimitive<Wm3::Box3f>
{
};
using CColBox = Moho::CColPrimitive<Wm3::Box3f>;

// 0x00E0D480
template<>
class CColPrimitive<Wm3::Sphere3f>
{
};
using CColSphere = Moho::CColPrimitive<Wm3::Sphere3f>;

}
