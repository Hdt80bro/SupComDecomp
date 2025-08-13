
namespace Moho {

// 0x00E20044
class CUnitCarrierRetrieve : public Moho::CCommandTask
{
public:
    int v0;
    int v1;
    Moho::EntitySetTemplate<Moho::Unit> v2;
};

// 0x00E20090
class CUnitCarrierLand : public Moho::CCommandTask
{
public:
    Moho::WeakPtr<Moho::Unit> v0;
    bool v2;
    int v3;
    float mHeight;
    Wm3::Vector3f mPos;
    Wm3::Vector3f mDir;
    Wm3::Vector3f v11;
};

// 0x00E200DC
class CUnitCarrierLaunch : public Moho::CCommandTask
{
public:
    Moho::SNavGoal mGoal;
    bool v9;
    Moho::EntitySetTemplate<Moho::Unit> v10;
};

}
