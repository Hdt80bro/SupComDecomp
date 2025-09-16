#include "core/ScriptObject.h"
#include "user/MauiEvent.h"

namespace Moho {

// 0x00E38DC0
class IMauiDragger : public Moho::TDatListItem<Moho::IMauiDragger, void>
{
public:
    virtual ~IMauiDragger() = default; // 0x0078DB90
    virtual void DragMove(Moho::SMauiEventData *) {} // 0x0078DB50
    virtual void DragRelease(Moho::SMauiEventData *) { delete this; } // 0x0078DB60
    virtual void DragCancel() { delete this; } // 0x0078DB80
};

class CMauiLuaDragger :
    public Moho::CScriptObject,
    public Moho::IMauiDragger
{
public:
    static gpg::RType *StaticGetClass(); // 0x0078DBD0
    gpg::RType *GetClass() const override; // 0x0078DBF0
    gpg::RRef GetDerivedObjectRef() override; // 0x0078DC10
    ~CMauiLuaDragger() override; // 0x0078DEC0
    //~CMauiLuaDragger(IMauiDragger) override; // 0x0078EBE0
    void DragMove(Moho::SMauiEventData *ev) override; // 0x0078DF30
    void DragRelease(Moho::SMauiEventData *ev) override; // 0x0078DF50
    void DragCancel() override; // 0x0078DF70
};

// 0x00E42514
class UIBuildDragger : public Moho::IMauiDragger
{
public:
    Moho::CWldSession *mWldSession;
    struct_WorldView_object *mWldViewObj;
    Moho::CameraImpl *mCam;
    Wm3::Vector3f mStart;
    Wm3::Vector3f mEnd;
    
    ~UIBuildDragger() override = default; // 0x00823030
    void DragMove(Moho::SMauiEventData *ev) override; // 0x00823BB0
    void DragRelease(Moho::SMauiEventData *ev) override; // 0x00823BD0
    void DragCancel() override { delete this; } // 0x00823CA0
};


int func_PostDragger(Moho::CMauiFrame *frame, Moho::IMauiDragger *dragger, const Moho::SMauiEventData &ev); // 0x0078DDE0

}
