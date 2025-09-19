#include "core/TDatList.h"
#include "core/MessageReceiver.h"

namespace Moho {

struct CMessageDispatcher
{
    Moho::TDatList<Moho::SMsgReceiverLinkage, void> mLinkageList;
    Moho::IMessageReceiver *mReceivers[256];

    void PushReceiver(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec); // 0x0047C360
    void RemoveLinkage(Moho::SMsgReceiverLinkage *); // 0x0047C450
    bool Dispatch(Moho::CMessage *msg); // 0x0047C4D0
};

}
