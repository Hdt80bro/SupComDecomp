#include "user/MauiControl.h"
#include "LuaPlus.h"
#include "boost/shared_ptr.hpp"

namespace Moho {

// 0x00E37E04
class CMauiBorder : public Moho::CMauiControl
{
public:
    boost::shared_ptr<Moho::CD3DBatchTexture> mTex1;
    boost::shared_ptr<Moho::CD3DBatchTexture> mTexHorz;
    boost::shared_ptr<Moho::CD3DBatchTexture> mTexUL;
    boost::shared_ptr<Moho::CD3DBatchTexture> mTexUR;
    boost::shared_ptr<Moho::CD3DBatchTexture> mTexLL;
    boost::shared_ptr<Moho::CD3DBatchTexture> mTexLR;
    LuaPlus::LuaObject mBorderWidthLV;
    LuaPlus::LuaObject mBorderHeightLV;

    static gpg::RType *StaticGetClass(); // 0x00784840
    gpg::RType *GetClass() const override; // 0x00784860
    gpg::RRef GetDerivedObjectRef() override; // 0x00784880
    ~CMauiBorder() override; // 0x00784B20
    void Draw(Moho::CD3DPrimBatcher *batcher, int renderPass) override; // 0x00784F50
    void Dump() override; // 0x00784D00

    void SetTextures(
        boost::shared_ptr<Moho::CD3DBatchTexture> *horz,
        boost::shared_ptr<Moho::CD3DBatchTexture> *vert,
        boost::shared_ptr<Moho::CD3DBatchTexture> *ul,
        boost::shared_ptr<Moho::CD3DBatchTexture> *ur,
        boost::shared_ptr<Moho::CD3DBatchTexture> *ll,
        boost::shared_ptr<Moho::CD3DBatchTexture> *lr
    ); // 0x00784D60
};


}