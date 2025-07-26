#include "core/Task.h"

namespace Moho {

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