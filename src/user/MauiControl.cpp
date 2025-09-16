#include "MauiControl.h"
#include "user/MauiFrame.h"


// 0x00782E90
gpg::RType *Moho::CMauiControl::StaticGetClass() {
    return func_GetType<Moho::CMauiControl>();
}

// 0x00786A60
Moho::CMauiControl::~CMauiControl() {
    this->Invalidate();
}

// 0x00786E90
void Moho::CMauiControl::DoInit() {
    this->RunScript("OnInit");
}

// 0x00786EF0
void Moho::CMauiControl::Destroy() {
    this->Invalidate();
    this->mInvisible = true;
    this->mParent = nullptr;
    if (this != this->mRootFrame) {
        this->mParentList.ListUnlink();
    }
    this->RunScript("OnDestroy");
    this->ClearChildren();
}

// 0x00787170
void Moho::CMauiControl::SetHidden(bool hidden) {
    if (! this->OnHide(hidden)) {
        this->mIsHidden = hidden;
        for (auto itr = this->mChildrenList.ListGetNext(); itr != &this->mChildren; itr = itr->) {
            itr->SetHidden(hidden);
        }
    }
}

// 0x007871C0
bool Moho::CMauiControl::IsHidden() {
    return this->mIsHidden;
}

// 0x00787270
bool Moho::CMauiControl::HitTest(float x, float y) {
    return this->Left() <= x && x < this->Right() &&
        this->Top() <= y && y < this->Bottom();
}

// 0x00787210
void Moho::CMauiControl::DisableHitTest(bool test, bool applyChildren) {
    this->mHitTest = test;
    if (applyChildren) {
        //... usual
    }
}

// 0x00787260
bool Moho::CMauiControl::IsHitTestDisabled() {
    return this->mHitTest;
}

// 0x007873A0
bool Moho::CMauiControl::HandleEvent(const Moho::SMauiEventData &ev) {
    LuaPlus::LuaObject obj = func_CreateLuaEvent(this->mLuaObj.GetActiveState(), ev);
    return this->RunScriptBool("HandleEvent", obj);
}

// 0x00787420
void Moho::CMauiControl::Frame(float delta) {
    this->RunScript("OnFrame", &delta);
}

// 0x00787460
void Moho::CMauiControl::AcquireKeyboardFocus(bool doAcquire) {
    Moho::MAUI_SetKeyboardFocus(this, doAcquire);
}

// 0x00787480
void Moho::CMauiControl::AbandonKeyboardFocus() {
    if (MAUI_CurrentKeyboardFocus.ListGetPrev() == this) {
        Moho::MAUI_SetKeyboardFocus(nullptr, true);
    }
}

// 0x00787440
void Moho::CMauiControl::LosingKeyboardFocus() {
    this->RunScript("OnLoseKeyboardFocus");
}

// 0x00787450
void Moho::CMauiControl::OnKeyboardFocusChange() {
    this->RunScript("OnKeyboardFocusChange");
}

// 0x007876D0
bool Moho::CMauiControl::IsScrollable(Moho::EMauiScrollAxis axis) {

}

// 0x007874B0
Moho::SMauiScrollValues Moho::CMauiControl::GetScrollValues(Moho::EMauiScrollAxis axis) {

}

// 0x00787780
void Moho::CMauiControl::ScrollLines(Moho::EMauiScrollAxis axis, float amt) {

}

// 0x00787830
void Moho::CMauiControl::ScrollPages(Moho::EMauiScrollAxis axis, float amt) {

}

// 0x007878E0
void Moho::CMauiControl::ScrollSetTop(Moho::EMauiScrollAxis axis, float amt) {

}

// 0x007871D0
void Moho::CMauiControl::OnMinimized(bool min) {

}

// 0x00786B40
void Moho::CMauiControl::Dump() {

}



// 0x0077F6F0
std::string Moho::CMauiControl::GetDebugName() const {
    return this->mDebugName;
}

// 0x0077F720
void Moho::CMauiControl::SetDebugName(std::string name) {
    this->mDebugName = name;
}

// 0x00786380
Moho::CMauiControl *Moho::CMauiControl::GetParent() const {
    return this->mParent;
}

// 0x00786390
Moho::CMauiFrame *Moho::CMauiControl::GetRootFrame() {
    return this->mRootFrame;
}

// 0x007863F0
void Moho::CMauiControl::SetAlpha(float alpha) {
    this->mAlpha = alpha;
    this->mVertexAlpha = this->AdjustARGBAlpha(0xFFFFFFFF);
}

// 0x00786440
float Moho::CMauiControl::GetAlpha() {
    return this->mAlpha;
}

// 0x00786460
void Moho::CMauiControl::SetRenderPass(int pass) {
    this->mRenderPass = pass;
}

// 0x00786470
int Moho::CMauiControl::GetRenderPass() {
    return this->mRenderPass;
}

// 0x00786480
bool Moho::CMauiControl::NeedsFrameUpdate() {
    return this->mNeedsFrameUpdate;
}

// 0x007867B0
Moho::CMauiControl::CMauiControl(LuaPlus::LuaObject &lObj, Moho::CMauiControl *parent, std::string debugName) :
    Moho::CScriptObject{},
    mParentList{},
    mChildrenList{},
    mLeft{lObj.m_state},
    mRight{lObj.m_state},
    mTop{lObj.m_state},
    mBottom{lObj.m_state},
    mWidth{lObj.m_state},
    mHeight{lObj.m_state},
    mDepth{lObj.m_state},
    mDepthValue{0.0},
    mRenderedChildren{},
    mInvalidated{true},
    mHitTest{false},
    mNeedsFrameUpdate{false},
    mInvisible{false},
    mAlpha{1.0},
    mVertexAlpha{-1},
    mRenderPass{0},
    mRootFrame{nullptr},
    mDebugName{}
{
    this->mDebugName = debugName;
    this->SetLuaObject(lObj);
    if (parent != nullptr) {
        this->mParentList.ListLinkBefore(&parent->mChildrenList);
        this->SetHidden(parent->IsHidden());
        this->mRenderPass = parent->mRenderPass;
        this->mRootFrame = parent->mRootFrame;
    }
    this->mLuaObj.SetObject("Left", this->mLeft);
    this->mLuaObj.SetObject("Right", this->mRight);
    this->mLuaObj.SetObject("Top", this->mTop);
    this->mLuaObj.SetObject("Bottom", this->mBottom);
    this->mLuaObj.SetObject("Width", this->mWidth);
    this->mLuaObj.SetObject("Height", this->mHeight);
    this->mLuaObj.SetObject("Depth", this->mDepth);
    this->mDepthValue = this->mDepth;
}

// 0x00786AA0
void Moho::CMauiControl::Invalidate() {
    this->mInvalidated = true;
    for (Moho::CMauiControl *i = this->mParent; i != nullptr; i = i->mParent) {
        i->mInvalidated = true;
    }
}

// 0x00786AD0
void Moho::CMauiControl::SetParent(Moho::CMauiControl *parent) {
    if (this->mParent != parent) {
        this->mInvalidated = true;
        if (parent != nullptr) {
            this->Invalidate();
        }
        this->mParentList.ListUnlink();
        this->mParent = parent;
        if (parent != nullptr) {
            this->mParentList.ListLinkBefore(&parent->mChildrenList);
            this->Invalidate();
        }
    }
}

// 0x00786EA0
Moho::CMauiControl *Moho::CMauiControl::DepthFirstSuccessor(Moho::CMauiControl *child) {

}

// 0x00786F60
void Moho::CMauiControl::ClearChildren() {

}

// 0x00786FA0
void Moho::CMauiControl::Render() {
    if (this->mInvisible) {
        return;
    }



}

// 0x007872E0
Moho::CMauiControl *Moho::CMauiControl::GetTopmostControl(float x, float y) {

}

// 0x00787370
void Moho::CMauiControl::PostEvent(const Moho::SMauiEventData &) {

}

// 0x00787990
void Moho::CMauiControl::ApplyFunction(LuaPlus::LuaObject func, bool applyChildren) {
    LuaPlus::LuaFunction<void> fn{func};
    fn(this->mLuaObj);
    if (applyChildren) {

    }
}

// 0x0078A700
bool Moho::CMauiControl::OnHide(bool &hidden) {
    return this->RunScriptBool("OnHide", hidden);
}

// 0x0078AA00
bool Moho::CMauiControl::GetIsScrollable() {
    return this->RunScriptBool("IsScrollable");
}

// 0x0078EC10
Moho::color_t Moho::CMauiControl::MultiplyAlpha(Moho::color_t col) {
    return (col & 0xFFFFFF) - ((unsigned int) (__int64) (this->mAlpha * -255.0) << 24);
}


// 0x0079CC10
void Moho::MAUI_SetKeyboardFocus(Moho::CMauiControl *ctrl, bool hasFocus) {
    // ...
    MAUI_ControlHasFocus = hasFocus;
    // ...
}

// 0x00795BD0
LuaPlus::LuaObject func_CreateLuaEvent(LuaPlus::LuaState *state, const Moho::SMauiEventData &ev) {
    using Moho::EMauiEventModifier;
    LuaPlus::LuaObject mods{};
    mods.AssignNewTable(state, 6, 0);
    if ((ev.mModifiers & MEM_Shift) != 0) {
        mods.SetBoolean("Shift", MEM_Shift);
    }
    if ((ev.mModifiers & MEM_Ctrl) != 0) {
        mods.SetBoolean("Ctrl", MEM_Shift);
    }
    if ((ev.mModifiers & MEM_Alt) != 0) {
        mods.SetBoolean("Alt", MEM_Shift);
    }
    if ((ev.mModifiers & MEM_Left) != 0) {
        mods.SetBoolean("Left", MEM_Shift);
    }
    if ((ev.mModifiers & MEM_Middle) != 0) {
        mods.SetBoolean("Middle", MEM_Shift);
    }
    if ((ev.mModifiers & MEM_Right) != 0) {
        mods.SetBoolean("Right", MEM_Shift);
    }
    LuaPlus::LuaObject luaEv{};
    luaEv.AssignNewTable(state, 0, 8);
    gpg::RRef evType{&ev.mEventType};
    luaEv.SetString("Type", evType.GetLexical().c_str());
    luaEv.SetNumber("MouseX", ev.mMouseX);
    luaEv.SetNumber("MouseY", ev.mMouseY);
    luaEv.SetInteger("WheelRotation", ev.mWheelRotation);
    luaEv.SetInteger("WheelDelta", ev.mWheelData);
    luaEv.SetInteger("KeyCode", ev.mKeyCode);
    luaEv.SetInteger("RawKeyCode", ev.mRawKeyCode);
    luaEv.SetObject("Modifiers", mods);
    if (ev.mSource != nullptr) {
        luaEv.SetObject("Control", ev.mSource->mLuaObj);
    }
    return luaEv;
}
