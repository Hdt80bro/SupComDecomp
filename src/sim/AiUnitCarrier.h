
namespace Moho {

struct SNavGoal
{
    gpg::Rect2i pos1;
    gpg::Rect2i pos2;
    Moho::ELayer layer;
};

class CUnitCarrierRetrieve : public Moho::CCommandTask
{
public:
    int v0;
    int v1;
    Moho::EntitySetTemplate_Unit v2;
};

class CUnitCarrierLand : public Moho::CCommandTask
{
public:
    Moho::WeakPtr<Moho::Unit> v0;
    bool v2;
    int v3;
    float height;
    Wm3::Vector3f pos;
    Wm3::Vector3f dir;
    Wm3::Vector3f v11;
};

class CUnitCarrierLaunch : public Moho::CCommandTask
{
public:
    Moho::SNavGoal v0;
    bool v9;
    Moho::EntitySetTemplate<Moho::Unit> v10;
};

}
