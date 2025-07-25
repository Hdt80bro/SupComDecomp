#include "TDatListItem.h"

namespace Moho {

struct STaskEventLinkage : Moho::TDatListItem<Moho::STaskEventLinkage>
{
    int v1;
    Moho::WeakPtr<Moho::CTaskThread> thread;
};

class CTaskEvent
{
public:
    bool v1;
    Moho::TDatListItem<Moho::STaskEventLinkage> v2;
};

}
