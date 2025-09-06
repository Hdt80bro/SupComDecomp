#include "core/TDatListItem.h"
#include "core/WeakPtr.h"

namespace Moho {

struct STaskEventLinkage : Moho::TDatListItem<Moho::STaskEventLinkage>
{
    int v1;
    Moho::WeakPtr<Moho::CTaskThread> mThread;
};

// 0x00E00150
class CTaskEvent
{
public:
    bool v1;
    Moho::TDatListItem<Moho::STaskEventLinkage> v2;
};

}
