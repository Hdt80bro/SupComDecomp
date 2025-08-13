
namespace Moho {

// 0x00E2E8C4
class CUnitCommand :
    public Moho::CScriptObject,
    public Moho::Broadcaster<Moho::ECommandEvent>
{
    int v2;
    Moho::Sim *mSim;
    Moho::SSTICommandConstantData mConstDat;
    Moho::SSTICommandVariableData mVarDat;
    int v46;
    Moho::EntitySetTemplate<Moho::Entity> mUnitSet;
    Moho::CAiFormationInstance *mFormationInstance;
    Moho::CAiTarget mTarget;
    int mCmdId;
    bool v67a;
    bool mNeedsUpdate;
    bool v67c;
    bool v67d;
    std::vector<Moho::WeakPtr<Moho::CUnitCommand>> mCoordinatingOrders;
    bool v72;
    Moho::WeakPtr<Moho::Unit> v73;
    LuaPlus::LuaObject mArgs;
    int v80;

    void AddUnit(std::vector<Moho::WeakPtr<Moho::CUnitCommand>> *queue, Moho::Unit *unit, int index);
};

}
