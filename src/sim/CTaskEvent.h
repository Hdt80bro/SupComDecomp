
namespace Moho {

    struct STaskEventLinkage : TDatListItem<Moho::STaskEventLinkage>
    {
        int v1;
        Moho::WeakPtr_CTaskThread thread;
    };

    class CTaskEvent
    {
        bool v1;
        Moho::TDatListItem<Moho::STaskEventLinkage> v2;
    };

};