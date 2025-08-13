#include "core/TDatListItem.h"
#include "core/Message.h"

namespace Moho {

struct CMessageDispatcher;

// 0x00E03BE4
class IMessageReceiver : public Moho::TDatListItem<Moho::IMessageReceiver>
{
public:
    virtual void Receive(Moho::CMessage *, Moho::CMessageDispatcher *) = 0;

    ~IMessageReceiver(); // 0x0047C4F0
};

struct struct_filler4 { int filler; };

struct SMsgReceiverLinkage :
    Moho::TDatListItem<Moho::SMsgReceiverLinkage>,
    struct_filler4,
    Moho::TDatListItem<Moho::IMessageReceiver>
{
    unsigned int mLower;
    unsigned int mUpper;
    Moho::IMessageReceiver *mReceiver;
    Moho::CMessageDispatcher *mDispatcher;

    SMsgReceiverLinkage(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec, Moho::CMessageDispatcher *disp) :
        mLower{lower},
        mUpper{upper},
        mReceiver{rec},
        mDispatcher{disp}
    {} // inline 0x0047C37A
};

}
