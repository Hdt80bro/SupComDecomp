#include <vector>

namespace Moho {

// 0x00E1B6AC
class IAiBuilder
{
    virtual ~IAiBuilder() = 0;
    virtual bool GetBool1() = 0;
    virtual void SetBool1(bool val) = 0;
    virtual void IssueRallyPoint() = 0;
    virtual void OnTick() = 0;
    virtual bool QueueIsEmpty() = 0;
    virtual bool GetBool2() = 0;
    virtual void SetBool2(bool val) = 0;
    virtual std::vector<Moho::WeakPtr<Moho::CUnitCommand>> GetCommands() = 0;
    virtual bool IsBusy() = 0;
    virtual void AddUnitToCommand(Moho::CUnitCommand *cmd, int comIndex) = 0;
    virtual bool HasCommand(Moho::CUnitCommand *cmd) = 0;
    virtual void* GetCommand(int idx) = 0;
    virtual void RemoveUnitToCommand(Moho::CUnitCommand *cmd) = 0;
    virtual void RemoveAllUnits() = 0;
    virtual void PrepareArmToBuild(Wm3::Vector3f) = 0;
    virtual Wm3::Vector3f* GetPos(Wm3::Vector3f *dest) = 0;
    virtual void SetBool3(bool val) = 0;
    virtual bool GetBool3() = 0;
    virtual void AddBuilding(Moho::SOCellPos *key, Moho::RUnitBlueprint *val) = 0;
    virtual void RemoveBuilding(Moho::SOCellPos *key) = 0;
    virtual void ClearBuildings() = 0;
    virtual Moho::RUnitBlueprint* GetFarthestBuilding(Moho::SOCellPos *dest) = 0;
};

// 0x00E1B73C
class CAiBuilderImpl : public Moho::IAiBuilder
{
public:
    Moho::Unit *unit;
    bool bool1;
    bool bool3;
    bool bool2;
    Wm3::Vector3f vec;
    std::map<uint, Moho::RUnitBlueprint> v6;
    std::vector<Moho::WeakPtr<Moho::CUnitCommand>> commands;
    
    ~CAiBuilderImpl() override = default; // 0x0059FB50
    bool GetBool1(); // 0x0059FAA0
    void SetBool1(bool val); // 0x0059FA90
    void IssueRallyPoint(); // 0x0059EEF0
    void OnTick(); // 0x0059F220
    bool QueueIsEmpty(); // 0x0059F440
    bool GetBool2(); // 0x0059EED0
    void SetBool2(bool val); // 0x0059EEE0
    std::vector<Moho::WeakPtr<Moho::CUnitCommand>> GetCommands(); // 0x0059F470
    bool IsBusy(); // 0x0059F480
    void AddUnitToCommand(Moho::CUnitCommand *cmd, int comIndex); // 0x0059F4D0
    bool HasCommand(Moho::CUnitCommand *cmd); // 0x0059F500
    void* GetCommand(int idx); // 0x0059F540
    void RemoveUnitToCommand(Moho::CUnitCommand *cmd); // 0059F580
    void RemoveAllUnits(); // 0x0059F5A0
    void PrepareArmToBuild(Wm3::Vector3f); // 0x0059F600
    Wm3::Vector3f* GetPos(Wm3::Vector3f *dest); // 0x0059F650
    void SetBool3(bool val); // 0x0059F670
    bool GetBool3(); // 0x0059F680
    void AddBuilding(Moho::SOCellPos *key, Moho::RUnitBlueprint *val); // 0x0059F690
    void RemoveBuilding(Moho::SOCellPos *key); // 0x0059F6C0
    void ClearBuildings(); // 0x0059F710
    Moho::RUnitBlueprint* GetFarthestBuilding(Moho::SOCellPos *dest); // 0x0059F740
};

Moho::IAiBuilder *AI_CreateBuilder(Moho::Unit *unit); // 0x0059FED0

}
