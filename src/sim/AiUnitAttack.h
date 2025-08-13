
namespace Moho {

class CAttackTargetTask :
    public Moho::CCommandTask, struct_filler4,
    public Moho::Listener<Moho::EAiAttackerEvent>,
    public Moho::Listener<Moho::ECommandEvent>
{
};


// 0x00E1F65C
// 0x00E1F668
// 0x00E1F670
class CUnitAttackTargetTask : public Moho::CAttackTargetTask
{
public:
    Moho::CUnitCommand *v28;
    Moho::CAiFormationInstance *mFormation;
    Moho::UnitWeapon *mWeapon;
    Moho::CAiTarget mTarget;
    Wm3::Vector3f v39;
    bool v42a;
    bool v42b;
    bool mIsGrounded;
    bool v42d;
};


// 0x00E20574
// 0x00E20580
// 0x00E20588
class CUnitMeleeAttackTargetTask : public Moho::CAttackTargetTask
{
public:
    int v2;
    int v3;
    Moho::CAiTarget aiTarget;
    Wm3::Vector3f pos;
    Moho::SOCellPos destination;
    bool v17;
    bool v18;
    bool v19;
    bool planted;
};

// 0x00E1E7C4
class IAiAttacker
{};

class CAiAttackerImpl :
    public Moho::IAiAttacker, // 0x00E1E9CC
    public Moho::TDatListItem<Moho::CAiAttackerImpl>,
    public Moho::CScriptObject // 0x00E1EA44
{
public:
    Moho::Unit *unit;
    Moho::CTaskStage stage;
    std::vector<Moho::UnitWeapon> weapons;
    Moho::WeakPtr<Moho::CTaskThread> thread;
    std::vector<Moho::CAcquireTargetTask> tasks;
    Moho::CAiTarget desiredTarget;
    Moho::CAiAttackerImpl::State reportingState;
};

// 0x00E1EA88
class LAiAttackerImpl : public Moho::CTask
{
public:
    int v1;
    Moho::CAiAttackerImpl *cImpl;
};

}
