#include <vector>

namespace Moho {

enum EDepositType
{
    DEPOTYPE_None = 0x0,
    DEPOTYPE_Mass = 0x1,
    DEPOTYPE_Hydrocarbon = 0x2,
};


struct ResourceDeposit
{
    gpg::Rect2i location;
    Moho::EDepositType type;

    bool Intersects(Moho::CGeomSolid3 *solid, Moho::CHeightField *field); // 0x00546170
};


class IResources
{
public:
    virtual ~IResources() = 0;
    virtual void AddDeposit(Moho::EDepositType type, gpg::Rect2i *pos) = 0;
    virtual void AddDepositPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) = 0;
    virtual const std::vector<Moho::ResourceDeposit> &GetDeposits() const = 0;
    virtual std::vector<Moho::ResourceDeposit> &GetDeposits() = 0;
    virtual bool IsDepositAt(gpg::Rect2i *pos, Moho::EDepositType type) = 0;
    virtual bool IsDepositAtPoint(Wm3::Vector3f *pos, Wm3::Vector2i *size, Moho::EDepositType type) = 0;
    virtual void DepositCollides(Moho::CGeomSolid3 *solid, int, gpg::fastvector<Moho::ResourceDeposit *> *dest, Moho::EDepositType type) = 0;
    virtual bool DepositIsInArea(Moho::EDepositType type, gpg::Rect2i *area) = 0;
    virtual bool DepositIsInAreaPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) = 0;
    virtual bool FindClosestDespoit(Moho::GridPos *from, Moho::GridPos *outPos, float radius, Moho::EDepositType type) = 0;
    virtual bool AreaHasDeposit(Moho::EDepositType type, gpg::Rect2f *area) = 0;
};

class CSimResources : public Moho::IResources
{
public:
    gpg::Mutex lock;
    std::vector<Moho::ResourceDeposit> deposits;

    ~CSimResources() override; // 0x00546A00
    void AddDeposit(Moho::EDepositType, gpg::Rect2i *pos) override; // 0x00545F10
    void AddDepositPoint(Moho::EDepositType, Wm3::Vector3f *pos, Wm3::Vector2i *size) override; // 0x00545E80
    const std::vector<Moho::ResourceDeposit> &GetDeposits() const override; // 0x00545FC0
    std::vector<Moho::ResourceDeposit> &GetDeposits() override; // 0x00545FB0
    bool IsDepositAt(gpg::Rect2i *pos, Moho::EDepositType type) override; // 0x00546060
    bool IsDepositAtPoint(Wm3::Vector3f *pos, Wm3::Vector2i *size, Moho::EDepositType type) override; // 0x00545FD0
    void DepositCollides(Moho::CGeomSolid3 *solid, Moho::CHeightField *field, gpg::fastvector<Moho::ResourceDeposit *> *dest, Moho::EDepositType type) override; // 0x00546470
    bool DepositIsInArea(Moho::EDepositType type, gpg::Rect2i *area) override; // 0x00546650
    bool DepositIsInAreaPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) override; // 0x005465C0
    bool FindClosestDespoit(Moho::GridPos *from, Moho::GridPos *outPos, float radius, Moho::EDepositType type) override; // 0x00546760
    bool AreaHasDeposit(Moho::EDepositType type, gpg::Rect2f *area) override; // 0x00546860
};  

}
