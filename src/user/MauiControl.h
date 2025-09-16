#include "user/MauiEvent.h"
#include "core/D3DRes.h"
#include "core/ScriptObject.h"
#include "LuaPlus.h"
#include <string>
#include <vector>

static Moho::TDatList<Moho::CScriptObject, void> MAUI_CurrentKeyboardFocus; // 0x010BD5AC
static bool MAUI_ControlHasFocus; // 0x00F58024

namespace Moho {

class CMauiFrame;

enum EMauiScrollAxis
{
    MSA_Vert = 0x0,
    MSA_Horz = 0x1, // does not work
};

struct SMauiScrollValues
{
    float mMinRange;
    float mMaxRange;
    float mMinVisible;
    float mMaxVisible;
};

// 0x00E3888C
class CMauiControl :
    public Moho::CScriptObject
{
public:
    Moho::TDatList<Moho::CMauiControl, void> mParentList;
    Moho::CMauiControl *mParent;
    Moho::TDatList<Moho::CMauiControl, void> mChildrenList;
    Moho::CScriptLazyVar<float> mLeft;
    Moho::CScriptLazyVar<float> mRight;
    Moho::CScriptLazyVar<float> mTop;
    Moho::CScriptLazyVar<float> mBottom;
    Moho::CScriptLazyVar<float> mWidth;
    Moho::CScriptLazyVar<float> mHeight;
    Moho::CScriptLazyVar<float> mDepth;
    float mDepthValue;
    std::vector<Moho::CMauiControl *> mRenderedChildren;
    bool mInvalidated;
    bool mHitTest;
    bool mIsHidden;
    bool mNeedsFrameUpdate;
    bool mInvisible;
    float mAlpha;
    unsigned int mVertexAlpha;
    int mRenderPass;
    Moho::CMauiFrame *mRootFrame;
    std::string mDebugName;

    static gpg::RType *StaticGetClass(); // 0x00782E90
    ~CMauiControl() override; // 0x00786A60
    virtual void DoInit(); // 0x00786E90
    virtual void Destroy(); // 0x00786EF0
    virtual void DoRender(Moho::CD3DPrimBatcher *batcher, unsigned int renderPass) {} // 0x00787160
    virtual void SetHidden(bool hidden); // 0x00787170
    virtual bool IsHidden(); // 0x007871C0
    virtual bool HitTest(float x, float y); // 0x00787270
    virtual void DisableHitTest(bool test, bool applyChildren); // 0x00787210
    virtual bool IsHitTestDisabled(); // 0x00787260
    virtual bool HandleEvent(const Moho::SMauiEventData &ev); // 0x007873A0
    virtual void Frame(float delta); // 0x00787420
    virtual void AcquireKeyboardFocus(bool doAcquire); // 0x00787460
    virtual void AbandonKeyboardFocus(); // 0x00787480
    virtual void LosingKeyboardFocus(); // 0x00787440
    virtual void OnKeyboardFocusChange(); // 0x00787450
    virtual bool IsScrollable(Moho::EMauiScrollAxis axis); // 0x007876D0
    virtual Moho::SMauiScrollValues GetScrollValues(Moho::EMauiScrollAxis axis); // 0x007874B0
    virtual void ScrollLines(Moho::EMauiScrollAxis axis, float amt); // 0x00787780
    virtual void ScrollPages(Moho::EMauiScrollAxis axis, float amt); // 0x00787830
    virtual void ScrollSetTop(Moho::EMauiScrollAxis axis, float amt); // 0x007878E0
    virtual void OnMinimized(bool); // 0x007871D0
    virtual void Dump(); // 0x00786B40

    std::string GetDebugName() const; // 0x0077F6F0
    void SetDebugName(std::string name); // 0x0077F720
    Moho::CMauiControl *GetParent() const; // 0x00786380
    Moho::CMauiFrame *GetRootFrame(); // 0x00786390
    void SetAlpha(float alpha); // 0x007863F0
    float GetAlpha(); // 0x00786440
    void SetRenderPass(int pass); // 0x00786460
    int GetRenderPass(); // 0x00786470
    bool NeedsFrameUpdate(); // 0x00786480
    CMauiControl(LuaPlus::LuaObject &lObj, Moho::CMauiControl *parent, std::string debugName); // 0x007867B0
    void Invalidate(); // 0x00786AA0
    void SetParent(Moho::CMauiControl *parent); // 0x00786AD0
    Moho::CMauiControl *DepthFirstSuccessor(Moho::CMauiControl *child); // 0x00786EA0
    void ClearChildren(); // 0x00786F60
    void Render(); // 0x00786FA0
    Moho::CMauiControl *GetTopmostControl(float, float); // 0x007872E0
    void PostEvent(const Moho::SMauiEventData &);// 0x00787370
    void ApplyFunction(LuaPlus::LuaObject func, bool applyChildren); // 0x00787990
    bool OnHide(bool &); // 0x0078A700
    bool GetIsScrollable(); // 0x0078AA00
    Moho::color_t AdjustARGBAlpha(Moho::color_t col); // 0x0078EC10

    Moho::CScriptLazyVar<float> &Left() {
        return this->mLeft;
    } // 0x0077F690
    Moho::CScriptLazyVar<float> &Right() {
        return this->mRight;
    } // 0x0077F6A0
    Moho::CScriptLazyVar<float> &Top() {
        return this->mTop;
    } // 0x0077F6B0
    Moho::CScriptLazyVar<float> &Bottom() {
        return this->mBottom;
    } // 0x0077F6C0
    Moho::CScriptLazyVar<float> &Width() {
        return this->mWidth;
    } // MohoEngine.dll
    Moho::CScriptLazyVar<float> &Height() {
        return this->mHeight;
    } // MohoEngine.dll
    Moho::CScriptLazyVar<float> &Depth() {
        return this->mDepth;
    } // MohoEngine.dll
};


void MAUI_SetKeyboardFocus(Moho::CMauiControl *ctrl, bool hasFocus); // 0x0079CC10

}

LuaPlus::LuaObject func_CreateLuaEvent(LuaPlus::LuaState *state, const Moho::SMauiEventData &ev); // 0x00795BD0
