#include "user/MauiControl.h"
//#include "wx/event.h"

namespace Moho {

class CMauiFrame;

// 0x00E3BA44
class CMauiWxEventMapper : public wxEvtHandler
{
public:
    int mVal;
    Moho::CMauiFrame *mFrame;

    ~CMauiWxEventMapper() override; // 0x007A48B0
    const wxEventTable *GetEventTable() const override; // 0x007A4530
};


class CMauiFrame : public Moho::CMauiControl
{
public:
    boost::shared_ptr<void> mPtr;
    Moho::TDatList<Moho::CMauiControl, void> mList;
    int mVal;
    Moho::CMauiWxEventMapper *mEventMapper;
    int mTargetHead;


    static gpg::RType *StaticGetClass(); // 0x00795FE0
    gpg::RType *GetClass() const override; // 0x00796000
    gpg::RRef GetDerivedObjectRef() override; // 0x00796020
    ~CMauiFrame() override; // 0x00796440
    void Frame(float delta) override; // 0x007965A0
    void Dump() override; // 0x00796720

  
    void SetBounds(int width, int height); // 0x00796550
};

}
