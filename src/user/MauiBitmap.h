#include "user/MauiControl.h"
#include "gpgcore/containers/Array2D.h"
#include "boost/shared_ptr.hpp"
#include <vector>

namespace Moho {

// 0x00E37B54
class CMauiBitmap : public Moho::CMauiControl
{
public:
    std::vector<boost::shared_ptr<Moho::CD3DBatchTexture>> mTextures;
    LuaPlus::LuaObject mBitmapWidthLV;
    LuaPlus::LuaObject mBitmapHeightLV;
    float mU0;
    float mV0;
    float mU1;
    float mV1;
    gpg::BitArray2D *mHitMask;
    bool mUseAlphaHitTest;
    bool mIsTiled;
    float mFrameRate;
    bool mIsPlaying;
    bool mDoLoop;
    int mCurFrame;
    float mCurFrameTime;
    std::vector<int> mFrames;
    
    static gpg::RType *StaticGetClass(); // 0x0077F780
    gpg::RType *GetClass() const override; // 0x0077F7A0
    gpg::RRef GetDerivedObjectRef() override; // 0x0077F7C0
    ~CMauiBitmap() override; // 0x0077FAA0
    void DoRender(Moho::CD3DPrimBatcher *batcher, unsigned int renderPass) override; // 0x00780850
    bool HitTest(float x, float y) override; // 0x0077FF70
    void Frame(float delta) override; // 0x00780270
    void Dump() override; // 0x0077FAD0

    CMauiBitmap(LuaPlus::LuaObject *lobj, Moho::CMauiControl *parent); // 0x0077F950
    void ShareTextures(Moho::CMauiBitmap *other); // 0x0077FCF0
    void SetTexture(boost::shared_ptr<Moho::CD3DBatchTexture> *tex); // 0x0077FD90
    void OnPatternEnd(); // 0x00780160
    void SetFramePattern(std::vector<int> *patt); /// 0x007802D0
    void SetForwardPattern(); // 0x00780420
    void SetBackwardPattern(); // 0x007804E0
    void SetPingPongPattern(); // 0x007805B0
    void SetLoopPingPongPattern(); // 0x00780700
};

}