#include "core/Task.h"
#include "core/TDatList.h"
#include "core/WeakPtr.h"

namespace Moho {

struct STaskEventLinkage : Moho::TDatListItem<Moho::STaskEventLinkage, void>
{
    int v1;
    Moho::WeakPtr<Moho::CTaskThread> mThread;
};

// 0x00E00150
class CTaskEvent
{
public:
    bool v1;
    Moho::TDatList<Moho::STaskEventLinkage, void> mLinks;
};

}
