// known file

#include "Wm3AxisAlignedBox3.h"
#include "Wm3Box3.h"
#include "Wm3Vector3.h"
#include "Wm3Sphere3.h"

struct struct_CollisionEntry
{
    int unk;
    Wm3::Vector3f mDir;
    Wm3::Vector3f mPos;
    float mDist;
};


namespace Moho {

struct CollisionResult
{
    int v0;
    Moho::Entity *mSource;
    Wm3::Vector3f mDir;
    float mDist;
};


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

    bool Collide(Moho::CColPrimitiveBase* with,  Moho::CollisionResult *res) {
        if (with->GetBox() != nullptr) {
            return this->CollideBox(with->GetBox(), res);
        } else {
            if (with->GetSphere() == nullptr) {
                gpg::HandleAssertFailure("Reached the supposably unreachable.", 94, "c:\\work\\rts\\main\\code\\src\\core/ColMain.h");
            }
            return this->CollideSphere(with->GetSphere(), res);
        }
    } // 0x00676A40
};


template <class T>
class CColPrimitive : public Moho::CColPrimitiveBase
{
public:
    T mShape;
    Wm3::Vector3f mCenter;

    
    Wm3::AxisAlignedBox3f GetBoundingBox() override {
        return Wm3::AxisAlignedBox3f{this->mShape};
    }
    void SetTransform(Moho::VTransform *) override {

    }
    Wm3::Vector3f GetCenter() override {
        return this->mCenter;
    }
    void SetCenter(Wm3::Vector3f *center) override {
        this->mCenter = *center;
    }
    bool CollideLine(Wm3::Vector3f *p1, Wm3::Vector3f *p2, struct_CollisionEntry *out) override {

    }
    bool CollideBox(Wm3::Box3f *box, Moho::CollisionResult *res) override {

    }
    bool CollideSphere(Wm3::Sphere3f *box, Moho::CollisionResult *res) override {

    }
    bool PointInShape(Wm3::Vector3f *p) override {

    }
};


// 0x00E0D50C
class CColPrimitive<Wm3::Box3f>
{
public:
    Wm3::AxisAlignedBox3f GetBoundingBox() override; // 0x004FFC20
    Wm3::Sphere3f *GetSphere() override; // 0x004FF130
    Wm3::Box3f *GetBox() override; // 0x004FF140
    void SetTransform(Moho::VTransform *) override; // 0x004FF470
    Wm3::Vector3f GetCenter() override; // 0x004FFBE0
    void SetCenter(Wm3::Vector3f *) override; // 0x004FFC00
    bool CollideLine(Wm3::Vector3f *p1, Wm3::Vector3f *p2, struct_CollisionEntry *out) override; // 0x004FF2D0
    bool CollideBox(Wm3::Box3f *box, Moho::CollisionResult *res) override; // 0x004FF260
    bool CollideSphere(Wm3::Sphere3f *box, Moho::CollisionResult *res) override; // 0x004FF150
    bool PointInShape(Wm3::Vector3f *p) override; // 0x004FF450
};
using CColBox = Moho::CColPrimitive<Wm3::Box3f>;

// 0x00E0D480
class CColPrimitive<Wm3::Sphere3f>
{
public:
    Wm3::AxisAlignedBox3f GetBoundingBox() override; // 0x004FF9A0
    Wm3::Sphere3f *GetSphere() override; // 0x004FE780
    Wm3::Box3f *GetBox() override; // 0x004FE790
    void SetTransform(Moho::VTransform *) override; // 0x004FEBC0
    Wm3::Vector3f GetCenter() override; // 0x004FF960
    void SetCenter(Wm3::Vector3f *) override; // 0x004FF980
    bool CollideLine(Wm3::Vector3f *p1, Wm3::Vector3f *p2, struct_CollisionEntry *out) override; // 0x004FE9D0
    bool CollideBox(Wm3::Box3f *box, Moho::CollisionResult *res) override; // 0x004FE860
    bool CollideSphere(Wm3::Sphere3f *box, Moho::CollisionResult *res) override; // 0x004FE7A0
    bool PointInShape(Wm3::Vector3f *p) override; // 0x004FEB60
};
using CColSphere = Moho::CColPrimitive<Wm3::Sphere3f>;

}
