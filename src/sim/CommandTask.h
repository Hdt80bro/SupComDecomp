#include "core/Task.h"

namespace Moho {

// 0x00E1B368
class CCommandTask : public Moho::CTask
{
    int v1;
    Moho::Unit *unit;
    Moho::Sim *sim;
    Moho::ETaskState task_state;
    int *dispatchLink_airesult;
    int link;
};

}