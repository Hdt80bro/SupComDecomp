#include "core/TDatListItem.h"

namespace Moho {

struct STaskEventLinkage : Moho::TDatListItem<Moho::STaskEventLinkage>
{
    int v1;
    Moho::WeakPtr<Moho::CTaskThread> thread;
};

// 0x00E00150
class CTaskEvent
{
public:
    bool v1;
    Moho::TDatListItem<Moho::STaskEventLinkage> v2;
};

}
