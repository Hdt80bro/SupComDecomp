
namespace Moho {

class CUnitCommand :
    public Moho::CScriptObject,
    public Moho::BroadCaster
{
    int v2;
    Moho::Sim *sim;
    Moho::SSTICommandConstantData stiConst;
    Moho::SSTICommandVariableData stiVar;
    int v46;
    Moho::EntitySetTemplate<Moho::Entity> unitSet;
    Moho::CAiFormationInstance *formationInstance;
    Moho::CAiTarget aiTarget;
    int cmdId;
    bool v67a;
    bool needsUpdate;
    bool v67c;
    bool v67d;
    std::vector<Moho::WeakPtr<Moho::CUnitCommand>> coordinatingOrders;
    bool v72;
    Moho::WeakPtr<Moho::Unit> v73;
    LuaPlus::LuaObject args;
    int v80;

    AddUnit(std::vector<Moho::WeakPtr<Moho::CUnitCommand>> *queue, Moho::Unit *unit, int index);
}

}
