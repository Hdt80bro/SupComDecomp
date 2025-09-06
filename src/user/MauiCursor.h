#include "user/MauiControl.h"
#include "boost/shared_ptr.hpp"

namespace Moho {

// 0x00E38B88
class CMauiCursor : public Moho::CScriptObject
{
public:
    boost::shared_ptr<Moho::CD3DBatchTexture> mTex;
    boost::shared_ptr<Moho::CD3DBatchTexture> mDefTex;
    bool mIsDef;
    bool mIsShowing;
    int mHotspotX;
    int mHotspotY;
    int mDefHotspotX;
    int mDefHotspotY;

    static gpg::RType *StaticGetClass(); // 0x0078C980
    gpg::RType *GetClass() const override; // 0x0078C9A0
    gpg::RRef GetDerivedObjectRef() override; // 0x0078C9C0
    ~CMauiCursor() override; // 0x0078CBD0

    void SetTexture(const char *filename); // 0x0078CCA0
    void SetDefaultTexture(const char *filename); // 0x0078CD80
    void ResetToDefault(); // 0x0078CEA0
};


}