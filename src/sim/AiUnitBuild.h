
namespace Moho {

    class IAiBuilder
    {
        virtual bool GetBool1();
        virtual void SetBool1(bool val);
        virtual void IssueRallyPoint();
        virtual void OnTick();
        virtual bool QueueIsEmpty();
        virtual bool GetBool2();
        virtual void SetBool2(bool val);
        virtual std::vector<Moho::WeakPtr<Moho::CUnitCommand>> GetCommands();
        virtual bool IsBusy();
        virtual void AddUnitToCommand(Moho::CUnitCommand *cmd, int comIndex);
        virtual bool HasCommand(Moho::CUnitCommand *cmd);
        virtual void* GetCommand(int idx);
        virtual void RemoveUnitToCommand(Moho::CUnitCommand *cmd);
        virtual void RemoveAllUnits();
        virtual void PrepareArmToBuild(Wm3::Vector3f);
        virtual Wm3::Vector3f* GetPos(Wm3::Vector3f *dest);
        virtual void SetBool3(bool val);
        virtual bool GetBool3();
        virtual void AddBuilding(Moho::SOCellPos *key, Moho::RUnitBlueprint *val);
        virtual void RemoveBuilding(Moho::SOCellPos *key);
        virtual void ClearBuildings();
        virtual Moho::RUnitBlueprint* GetFarthestBuilding(Moho::SOCellPos *dest);
    };

    class CAiBuilderImpl : IAiBuilder
    {
        Moho::Unit *unit;
        bool bool1;
        bool bool3;
        bool bool2;
        Wm3::Vector3f vec;
        std::map<uint, Moho::RUnitBlueprint> v6;
        std::vector<Moho::WeakPtr<Moho::CUnitCommand>> commands;
        
        bool GetBool1() asm("0x0059FAA0");
        void SetBool1(bool val) asm("0x0059FA90");
        void IssueRallyPoint() asm("0x0059EEF0");
        void OnTick() asm("0x0059F220");
        bool QueueIsEmpty() asm("0x0059F440");
        bool GetBool2() asm("0x0059EED0");
        void SetBool2(bool val) asm("0x0059EEE0");
        std::vector<Moho::WeakPtr<Moho::CUnitCommand>> GetCommands() asm("0x0059F470");
        bool IsBusy() asm("0x0059F480");
        void AddUnitToCommand(Moho::CUnitCommand *cmd, int comIndex) asm("0x0059F4D0");
        bool HasCommand(Moho::CUnitCommand *cmd) asm("0x0059F500");
        void* GetCommand(int idx) asm("0x0059F540");
        void RemoveUnitToCommand(Moho::CUnitCommand *cmd) asm("0059F580");
        void RemoveAllUnits() asm("0x0059F5A0");
        void PrepareArmToBuild(Wm3::Vector3f) asm("0x0059F600");
        Wm3::Vector3f* GetPos(Wm3::Vector3f *dest) asm("0x0059F650");
        void SetBool3(bool val) asm("0x0059F670");
        bool GetBool3() asm("0x0059F680");
        void AddBuilding(Moho::SOCellPos *key, Moho::RUnitBlueprint *val) asm("0x0059F690");
        void RemoveBuilding(Moho::SOCellPos *key) asm("0x0059F6C0");
        void ClearBuildings() asm("0x0059F710");
        Moho::RUnitBlueprint* GetFarthestBuilding(Moho::SOCellPos *dest) asm("0x0059F740");
    };

    Moho::IAiBuilder *AI_CreateBuilder(Moho::Unit *unit) asm("0x0059FED0");

};