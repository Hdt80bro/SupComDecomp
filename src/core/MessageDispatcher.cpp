#include "MessageDispatcher.h"

// 0x0047C360
void Moho::CMessageDispatcher::PushReceiver(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec) {
    auto linkage = new Moho::SMsgReceiverLinkage{lower, upper, rec, this};
    linkage->mReceiverLinkages.ListLinkBefore(&this->mLinkageList);
    linkage->mReceivers.ListLinkBefore(&rec->mReceiverList);
    if (lower < upper) {
        for (int k = lower; k <= upper; ++k) {
            this->mReceivers[k] = rec;
        }
    }
}

// 0x0047C450
void Moho::CMessageDispatcher::RemoveLinkage(Moho::SMsgReceiverLinkage *linkage) {
    int lower = linkage->mLower;
    Moho::IMessageReceiver **cur = &this->mReceivers[lower];
    while (lower < linkage->mUpper) {
        if (*cur == linkage->mReceiver) {
            *cur = nullptr;
            for (auto i = linkage->mReceiverLinkages.begin(); i != linkage->mReceiverLinkages.end(); ++i) {
                Moho::SMsgReceiverLinkage *msg = *i;
                if (msg->mLower <= lower && lower < msg->mUpper) {
                    *cur = msg->mReceiver;
                }
            }
        }
        ++lower;
        ++cur;
    }
    delete(linkage);
}
