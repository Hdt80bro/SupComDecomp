#include "MessageReceiver.h"
#include "core/MessageDispatcher.h"

// 0x0047C4F0
Moho::IMessageReceiver::~IMessageReceiver() {
    while (! this->mReceiverList.ListEmpty()) {
        auto linked = static_cast<Moho::SMsgReceiverLinkage *>(this->mReceiverList.ListGetNext());
        linked->mDispatcher->RemoveLinkage(linked);
    }
}
