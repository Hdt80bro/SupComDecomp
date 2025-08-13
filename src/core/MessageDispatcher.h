#include "core/TDatListItem.h"
#include "core/MessageReceiver.h"

namespace Moho {

struct CMessageDispatcher : Moho::TDatListItem<Moho::SMsgReceiverLinkage>
{
    Moho::IMessageReceiver *mReceivers[256];

    void PushReceiver(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec); // 0x0047C360
    void RemoveLinkage(Moho::SMsgReceiverLinkage *); // 0x0047C450
};

}
