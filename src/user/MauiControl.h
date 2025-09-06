#include "user/MauiEvent.h"
#include "core/ScriptObject.h"
#include "LuaPlus.h"
#include <string>
#include <vector>


float func_GetLazyValue(LuaPlus::LuaObject *obj); // 0x00783840

namespace Moho {

static Moho::TDatListItem<Moho::CScriptObject> Maui_CurrentFocusControl; // 0x010BD5AC
static bool Maui_ControlHasFocus; // 0x00F58024


class CMauiFrame;

enum EMauiScrollAxis
{
    MSA_Vert = 0x0,
    MSA_Horz = 0x1, // does not work
};

struct SScrollValues
{
    float mMinRange;
    float mMaxRange;
    float mMinVisible;
    float mMaxVisible;
};

// 0x00E3888C
class CMauiControl :
    public Moho::CScriptObject,
    public Moho::TDatListItem<Moho::CMauiControl>
{
public:
    Moho::CMauiControl *mParent;
    Moho::TDatListItem<Moho::CMauiControl> mChildren;
    LuaPlus::LuaObject mLeftLV;
    LuaPlus::LuaObject mRightLV;
    LuaPlus::LuaObject mTopLV;
    LuaPlus::LuaObject mBottomLV;
    LuaPlus::LuaObject mWidthLV;
    LuaPlus::LuaObject mHeightLV;
    LuaPlus::LuaObject mDepthLV;
    float mDepth;
    std::vector<Moho::CMauiControl*> mRenderedChildren;
    bool mInvalidated;
    bool mHitTest;
    bool mIsHidden;
    bool mNeedsFrameUpdate;
    bool mInvisible;
    float mAlpha;
    unsigned int mAlphaMask;
    int mRenderPass;
    Moho::CMauiFrame *mRootFrame;
    std::string mDebugName;

    static gpg::RType *StaticGetClass(); // 0x00782E90
    ~CMauiControl() override; // 0x00786A60
    virtual void OnInit(); // 0x00786E90
    virtual void Destroy(); // 0x00786EF0
    virtual void Draw(Moho::CD3DPrimBatcher *batcher, int renderPass) {} // 0x00787160
    virtual void SetHidden(bool hidden); // 0x00787170
    virtual bool IsHidden(); // 0x007871C0
    virtual bool HitTest(float x, float y); // 0x00787270
    virtual void SetHitTest(bool test, bool applyChildren); // 0x00787210
    virtual bool DoesHitTest(); // 0x00787260
    virtual bool HandleEvent(const Moho::SMauiEventData &ev); // 0x007873A0
    virtual void OnFrame(float delta); // 0x00787420
    virtual void AcquireFocus(bool doAcquire); // 0x00787460
    virtual void LoseFocus(); // 0x00787480
    virtual void LoseKeyboardFocus(); // 0x00787440
    virtual void KeyoardFocusChange(); // 0x00787450
    virtual bool IsScrollable(Moho::EMauiScrollAxis axis); // 0x007876D0
    virtual void GetScrollValues(Moho::EMauiScrollAxis axis); // 0x007874B0
    virtual void ScrollLines(Moho::EMauiScrollAxis axis, float amt) const; // 0x00787780
    virtual void ScrollLines(Moho::EMauiScrollAxis axis, float amt); // 0x00787830
    virtual void ScrollSetTop(Moho::EMauiScrollAxis axis, float amt); // 0x007878E0
    virtual Moho::CMauiControl *LastChild(int); // 0x007871D0
    virtual void Dump(); // 0x00786B40

    std::string GetName() const; // 0x0077F6F0
    void SetName(std::string *name); // 0x0077F720
    void SetAlpha(float alpha); // 0x007863F0
    CMauiControl(LuaPlus::LuaObject *lObj, Moho::CMauiControl *parent, std::string debugName); // 0x007867B0
    void Invalidate(); // 0x00786AA0
    void SetParent(Moho::CMauiControl *parent); // 0x00786AD0
    Moho::CMauiControl *NextChild(Moho::CMauiControl *child); // 0x00786EA0
    void ClearChildren(); // 0x00786F60
    void Validate(); // 0x00786FA0
    Moho::CMauiControl *LastChild(int); // 0x007871D0
    void ApplyFunction(LuaPlus::LuaObject func); // 0x00787990
    void SetSize(int width, int height); // 0x00796550

    float GetLeft() {
        return func_GetLazyValue(&this->mLeftLV);
    }
    float GetRight() {
        return func_GetLazyValue(&this->mRightLV);
    }
    float GetTop() {
        return func_GetLazyValue(&this->mTopLV);
    }
    float GetBottom() {
        return func_GetLazyValue(&this->mBottomLV);
    }
    float GetWidth() {
        return func_GetLazyValue(&this->mWidthLV);
    }
    float GetHeight() {
        return func_GetLazyValue(&this->mHeightLV);
    }
};

}

LuaPlus::LuaObject func_CreateLuaEvent(LuaPlus::LuaState *state, const Moho::SMauiEventData &ev); // 0x00795BD0
void func_AcquireFocus(Moho::CMauiControl *ctrl, bool hasFocus); // 0x0079CC10
