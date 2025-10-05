#include "core/TDatList.h"
#include "core/Message.h"

namespace Moho {

struct CMessageDispatcher;

// 0x00E03BE4
class IMessageReceiver
{
public:
    Moho::TDatList<Moho::IMessageReceiver, void> mReceiverList;

    virtual void ReceiveMessage(Moho::CMessage *, Moho::CMessageDispatcher *) = 0;

    ~IMessageReceiver(); // 0x0047C4F0
};

struct SMsgReceiverLinkage
{
    Moho::TDatList<Moho::SMsgReceiverLinkage, void> mReceiverLinkages;
    int gap;
    Moho::TDatList<Moho::IMessageReceiver, void> mReceivers;
    unsigned int mLower;
    unsigned int mUpper;
    Moho::IMessageReceiver *mReceiver;
    Moho::CMessageDispatcher *mDispatcher;

    SMsgReceiverLinkage(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec, Moho::CMessageDispatcher *disp) :
        mReceiverLinkages{},
        mReceivers{},
        mLower{lower},
        mUpper{upper},
        mReceiver{rec},
        mDispatcher{disp}
    {} // inline 0x0047C37A
};

}
