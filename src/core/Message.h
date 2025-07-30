#include "gpgcore/containers/fastvector.h"
#include "core/TDatListItem.h"

namespace Moho {

struct CMessage
{
    gpg::fastvector_n<char, 64> buf;
    int pos;

    void SetSize(int size) {
        this->buf[1] = LOBYTE(size);
        this->buf[2] = HIBYTE(size);
    } // inline e.g. 0x0047BE62
    unsigned short GetSize() {
        // return *(unsigned short *)(&this->buf[1]);
        return MAKEWORD(this->buf[1], this->buf[2]);
    } // inline e.g. 0x0047BF4C
    bool HasReadLength() {
        return this->pos >= 3;
    } // inline e.g. 0x0047BEE5
    char GetType() {
        return this->buf[0];
    } // inline e.g. 0x007BFB97
    void SetType(char type) {
        this->buf[0] = type;
    } // inline e.g. 0x004834E9

    CMessage(int size, char type); // 0x00483490
    int GetMessageSize(); // 0x0047BE90
    bool ReadMessage(gpg::Stream *stream); // 0x0047BD40
    bool Read(gpg::Stream *stream); // 0x0047BEE0
    unsigned int Append(char *ptr, int size); // 0x0047BDE0
};

struct SMsgReceiverLinkage;

// 0x00E03BE4
class IMessageReceiver : public Moho::TDatListItem<Moho::IMessageReceiver>
{
public:
    virtual void Receive(Moho::CMessage *, Moho::CMessageDispatcher *) = 0;

    ~IMessageReceiver(); // 0x0047C4F0
};

struct CMessageDispatcher : Moho::TDatListItem<Moho::SMsgReceiverLinkage>
{
    Moho::IMessageReceiver *receivers[256];

    void PushReceiver(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec); // 0x0047C360
    void RemoveLinkage(Moho::SMsgReceiverLinkage *); // 0x0047C450
};

struct struct_filler4 { int filler; };

struct SMsgReceiverLinkage :
    Moho::TDatListItem<Moho::SMsgReceiverLinkage>,
    struct_filler4,
    Moho::TDatListItem<Moho::IMessageReceiver>
{
    unsigned int lower;
    unsigned int upper;
    Moho::IMessageReceiver *rec;
    Moho::CMessageDispatcher *dispatcher;

    SMsgReceiverLinkage(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec, Moho::CMessageDispatcher *disp) :
        lower{lower},
        upper{upper},
        rec{rec},
        dispatcher{disp}
    {} // inline 0x0047C37A
};


}
