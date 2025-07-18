#include "TDatListItem.h"
#include "boost/function.hpp"

namespace Moho {

class CTaskThread;

class CTask
{
public:
    int v1;
    bool *destroyed;
    Moho::CTaskThread *taskThread;
    Moho::CTask *subtask;
    bool isOwning;

    virtual ~CTask(); // 0x00408C90
    virtual int Dispatch() = 0;

    CTask(Moho::CTaskThread *, bool); // 0x00408C40
    void TaskInterruptSubtasks(); // 0x00408D70
    void TaskResume(int a2, bool interrupt); // 0x00408DB0
};

struct CTaskStage
{
    Moho::TDatListItem<Moho::CTaskThread> threads;
    Moho::CTaskThread *prev2;
    Moho::CTaskThread *next2;
    bool v5;
};


struct CTaskThread : Moho::TDatListItem<Moho::CTaskThread>
{
    int unk;
    Moho::CTaskStage *stage;
    Moho::CTask *task;
    int val1;
    bool staged;

    void Destroy(); //0x00409190
    void Stage(); // 0x004093E0
    void Unstage(); // inline e.g. 0x004091C0
};


}
