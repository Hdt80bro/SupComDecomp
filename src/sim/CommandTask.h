#include "core/Task.h"

namespace Moho {

// 0x00E1B368
class CCommandTask : public Moho::CTask
{
    int v1;
    Moho::Unit *mUnit;
    Moho::Sim *mSim;
    Moho::ETaskState mTaskState;
    int *mDispatchLink_airesult;
    int mLink;
};

}