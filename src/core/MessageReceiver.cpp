#include "MessageReceiver.h"
#include "core/MessageDispatcher.h"

// 0x0047C4F0
Moho::IMessageReceiver::~IMessageReceiver() {
    while (this->mNext != this) {
        auto linked = static_cast<Moho::SMsgReceiverLinkage *>(this->mNext);
        linked->mDispatcher->RemoveLinkage(linked);
    }
}
