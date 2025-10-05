#include "core/TDatList.h"
#include "boost/function.hpp"

namespace Moho {

enum ETaskState
{
    TASKSTATE_Preparing = 0x0,
    TASKSTATE_Waiting = 0x1,
    TASKSTATE_Starting = 0x2,
    TASKSTATE_Processing = 0x3,
    TASKSTATE_Complete = 0x4,
    TASKSTATE_5 = 0x5,
    TASKSTATE_6 = 0x6,
    TASKSTATE_7 = 0x7,
    TASKSTATE_8 = 0x8,
};


class CTaskThread;

// likely doesn't exist and everyone inherits `CTask`, but I need
// a base for all of the tasks so I can give them all distinctly named `Execute` methods
struct ITask
{
    int v1;
    bool *mDestroyed;
    Moho::CTaskThread *mTaskThread;
    Moho::CTask *mSubtask;
    bool mIsOwning;
};

// 0x00E0031C
class CTask : public Moho::ITask
{
public:
    virtual ~CTask(); // 0x00408C90
    virtual int Execute() = 0;

    CTask(Moho::CTaskThread *, bool); // 0x00408C40
    void TaskInterruptSubtasks(); // 0x00408D70
    void TaskResume(int a2, bool interrupt); // 0x00408DB0
};

template<class T>
class CPushTask : public Moho::ITask
{
public:
    virtual ~CPushTask();
    virtual int Push() = 0;
};

template<class T>
class CPullTask : public Moho::ITask
{
public:
    virtual ~CPullTask();
    virtual int Pull() = 0;
};

struct CTaskStage
{
    Moho::TDatList<Moho::CTaskThread, void> mThreads;
    Moho::CTaskThread *mPrev;
    Moho::CTaskThread *mNext;
    bool v5;
};


struct CTaskThread : Moho::TDatListItem<Moho::CTaskThread, void>
{
    int unk;
    Moho::CTaskStage *mStage;
    Moho::CTask *mTask;
    int val1;
    bool mStaged;

    void Destroy(); //0x00409190
    void Stage(); // 0x004093E0
    void Unstage(); // inline e.g. 0x004091C0
};


}
