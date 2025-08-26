#include "core/ScriptObject.h"
#include "LuaPlus.h"
#include <string>
#include <vector>

namespace Moho {

class CMauiFrame;

// 0x00E3888C
class CMauiControl :
    public Moho::CScriptObject,
    public Moho::TDatListItem<Moho::CMauiControl>
{
public:
    Moho::CMauiControl *mParent;
    Moho::TDatListItem<Moho::CMauiControl> mChildrenLL;
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

    ~CMauiControl() override; // 0x00786A60
    virtual void OnInit(); // 0x00786E90
    virtual void Destroy(); // 0x00786EF0
    virtual void Draw(); // 0x00787160
    virtual void SetHidden(); // 0x00787170
    virtual void IsHidden(); // 0x007871C0
    virtual void HitTest(); // 0x00787270
    virtual void SetHitTest(); // 0x00787210
    virtual void DoesHitTest(); // 0x00787260
    virtual void HandleEvent(); // 0x007873A0
    virtual void OnFrame(); // 0x00787420
    virtual void AcquireFocus(); // 0x00787460
    virtual void LoseFocus(); // 0x00787480
    virtual void LoseKeyboardFocus(); // 0x00787440
    virtual void KeyoardFocusChange(); // 0x00787450
    virtual void IsScrollable(); // 0x007876D0
    virtual void GetScrollValues(); // 0x007874B0
    virtual void ScrollLines() const; // 0x00787780
    virtual void ScrollLines(); // 0x00787830
    virtual void ScrollSetTop(); // 0x007878E0
    virtual void LastChild(); // 0x007871D0
    virtual void Dump(); // 0x00786B40
};

}
