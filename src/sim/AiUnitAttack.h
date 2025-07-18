
namespace Moho {

class CAttackTargetTask :
    public Moho::CCommandTask, struct_filler4,
    public Moho::Listener<Moho::EAiAttackerEvent>,
    public Moho::Listener<Moho::ECommandEvent>
{
};


class CUnitAttackTargetTask : public Moho::CAttackTargetTask
{
public:
    Moho::CUnitCommand *v28;
    Moho::CAiFormationInstance *formation;
    Moho::UnitWeapon *weapon;
    Moho::CAiTarget target;
    Wm3::Vector3f v39;
    bool v42a;
    bool v42b;
    bool isGrounded;
    bool v42d;
};

class CAiAttackerImpl
{
public:
    int v0;
    int v1;
    Moho::CScriptObject scriptobj;
    Moho::Unit *unit;
    Moho::CTaskStage stage;
    std::vector<Moho::UnitWeapon> weapons;
    Moho::WeakPtr<Moho::CTaskThread> thread;
    std::vector<Moho::CAcquireTargetTask> tasks;
    Moho::CAiTarget desiredTarget;
    Moho::CAiAttackerImpl::State reportingState;
};

class LAiAttackerImpl : public Moho::CTask
{
public:
    int v1;
    Moho::CAiAttackerImpl *cImpl;
};

}
