
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

        bool Intersects(Moho::CGeomSolid3 *solid, Moho::CHeightField *field) asm("0x00546170");
    };


    class IResources
    {
        virtual ~IResources();
        virtual void AddDeposit(Moho::EDepositType type, gpg::Rect2i *pos);
        virtual void AddDepositPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size);
        virtual const std::vector<Moho::ResourceDeposit>& GetDeposits() const;
        virtual std::vector<Moho::ResourceDeposit>& GetDeposits();
        virtual bool IsDepositAt(gpg::Rect2i *pos, Moho::EDepositType type);
        virtual bool IsDepositAtPoint(Wm3::Vector3f *pos, Wm3::Vector2i *size, Moho::EDepositType type);
        virtual void DepositCollides(Moho::CGeomSolid3 *solid, int, gpg::fastvector<Moho::ResourceDeposit*> *dest, Moho::EDepositType type);
        virtual bool DepositIsInArea(Moho::EDepositType type, gpg::Rect2i *area);
        virtual bool DepositIsInAreaPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size);
        virtual bool FindClosestDespoit(Moho::GridPos *from, Moho::GridPos *outPos, float radius, Moho::EDepositType type);
        virtual bool AreaFreeOfDeposit(Moho::EDepositType type, gpg::Rect2f *area);
    };

    class CSimResources : Moho::IResources
    {
        gpg::Mutex lock;
        std::vector<Moho::ResourceDeposit> deposits;

        void ~CSimResources() override asm("0x00546A00");
        void AddDeposit(Moho::EDepositType, gpg::Rect2i *pos) override asm("0x00545F10");
        void AddDepositPoint(Moho::EDepositType, Wm3::Vector3f *pos, Wm3::Vector2i *size) override asm("0x00545E80");
        const std::vector<Moho::ResourceDeposit>& GetDeposits() const override asm("0x00545FC0");
        std::vector<Moho::ResourceDeposit>& GetDeposits() override asm("0x00545FB0");
        bool IsDepositAt(gpg::Rect2i *pos, Moho::EDepositType type) override asm("0x00546060");
        bool IsDepositAtPoint(Wm3::Vector3f *pos, Wm3::Vector2i *size, Moho::EDepositType type) override asm("0x00545FD0");
        void DepositCollides(Moho::CGeomSolid3 *solid, Moho::CHeightField *field, gpg::fastvector<Moho::ResourceDeposit*> *dest, Moho::EDepositType type) override asm("0x00546470");
        bool DepositIsInArea(Moho::EDepositType type, gpg::Rect2i *area) override asm("0x00546650");
        bool DepositIsInAreaPoint(Moho::EDepositType type, Wm3::Vector3f *pos, Wm3::Vector2i *size) override asm("0x005465C0");
        bool FindClosestDespoit(Moho::GridPos *from, Moho::GridPos *outPos, float radius, Moho::EDepositType type) override asm("0x00546760");
        bool AreaFreeOfDeposit(Moho::EDepositType type, gpg::Rect2f *area) override asm("0x00546860");
    };  

};