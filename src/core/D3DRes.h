#include "core/Counted.h"
#include "core/resources/ResourceFactory.h"
#include "gpgcore/MemBuffer.h"
#include "gpggal/Math.h"
#include "Wm3Vector2.h"
#include "Wm3Vector3.h"
#include "boost/shared_ptr.hpp"
#include <vector>

namespace Moho {

struct struct_FontMetrics
{
    float mHeight;
    float mAscent;
    float mDescent;
    float mInternalLeading;
    float mExternalLeading;
    float mAveCharWidth;
    float mOverhang;
};


// 0x00E01934
class CD3DFont : public Moho::CountedObject
{
public:
    static Moho::CountedPtr<class Moho::CD3DFont> Create(int, gpg::StrArg); // 0x00425290

    struct SCharInfo
    {
        boost::shared_ptr<Moho::CD3DBatchTexture> mTex;
        float mV2;
        float mV3;
        float mV4;
        float mV5;
        float mAdvance;
        int mV7;
        int mV8;
    };


public:
    HFONT mFont;
    bool mIsTruetype;
    struct_FontMetrics mFontMetrics;
    std::vector<std::vector<Moho::CD3DFont::SCharInfo>> mCharInfo;
    std::vector<int> mKerningPairs;
    HDC mDeviceContext;
    HBITMAP mBitmap;

    ~CD3DFont(); // 0x00425840
    const Moho::CD3DFont::SCharInfo &GetCharInfo(wchar_t c); // 0x00425940
    Wm3::Vector3f Render(gpg::StrArg, Moho::CD3DPrimBatcher *, const Wm3::Vector3f &, const Wm3::Vector3f &, const Wm3::Vector3f &, unsigned int, float, float); // 0x00426470
    void Render2D(gpg::StrArg, Moho::CD3DPrimBatcher *, const Wm3::Vector2f &, unsigned int, float, float); // 0x00426580
    float GetAdvance(gpg::StrArg, int); // 0x00426610
    int GetNearestCharacterIndex(gpg::StrArg, float); // 0x00426680
};

// 0x00E027F4
class CD3DIndexSheet
{
public:
    struct View
    {
        Moho::CD3DIndexSheet *mSheet;
        int mStart;
        int mEnd;
    };


};

// 0x00E02834
class CD3DVertexSheet
{
public:
    struct View
    {
        Moho::CD3DIndexSheet *mSheet;
        int mBaseVertIndex;
        int mStart;
        int mEnd;
    };

};

// 0x00E02820
class CD3DVertexStream
{

};

// 0x00E027C4
class CD3DRenderTarget
{

};

// 0x00E027D4
class CD3DDepthStencil : public Moho::CD3DRenderTarget
{

};

// 0x00E02798
class CD3DDynamicTextureSheet
{

};

// 0x00E02A84
class CD3DBatchTexture
{
public:
    virtual ~CD3DBatchTexture() = default; // 0x00447170
    virtual void Func() = 0;
    virtual float GetAlphaAt() = 0;
};

// 0x00E02A9C
class CD3DSolidBatchTexture : public Moho::CD3DBatchTexture
{

};

// 0x00E02B34
class CD3DRawBatchTexture
{

};

// 0x00E02AAC
class CD3DFileBatchTexture : public Moho::CD3DBatchTexture
{

};

// 0x00E027E4
class CD3DVertexFormat
{
public:
    boost::shared_ptr<gpg::gal::VertexFormatD3D9> mFormat;
};

struct CD3DEffect
{
public:
    // 0x00E02204
    class Technique
    {
    public:
        // 0x00E0220C
        class Implementation
        {

        };

        std::string mName;
        Moho::CD3DEffect::Technique::Implementation mImpl[3];
    };


};

// 0x00E02938
class CD3DTextureResourceFactory :
    public Moho::ResourceFactoryPreload<Moho::RD3DTextureResource, gpg::MemBuffer<const char>> // 0x00E02958
{

};

struct CD3DTextureBatcher
{
    int mWidth;
    int mHeight;
    std::vector<gpg::Rect2i> mRects;
    boost::shared_ptr<Moho::CD3DDynamicTextureSheet> mDynTexSheet;
    std::map<unk_t> mMap;
    bool mBool;
    std::vector<unk_t> mVec;
};


struct CD3DPrimBatcher
{
    struct Vertex
    {
        Wm3::Vector3f mV0;
        Wm3::Vector3f mV1;
    };

    Moho::CD3DTextureBatcher *mTextureBatcher;
    Moho::CD3DVertexSheet *mVertexSheets[3];
    int mCurVertexSheet;
    Moho::CD3DIndexSheet *mIndexSheet;
    std::vector<Moho::CD3DPrimBatcher::Vertex> mVertices;
    std::vector<short> mPrimitives;
    int mMode;
    boost::shared_ptr<Moho::CD3DDynamicTextureSheet> mDynamicTexSheet;
    boost::shared_ptr<Moho::CD3DBatchTexture> mTexture;
    Wm3::Vector2f mP2;
    Wm3::Vector2f mP1;
    gpg::gal::Matrix mViewMatrix;
    gpg::gal::Matrix mProjectionMatrix;
    gpg::gal::Matrix mComposite;
    bool mResetComposite;
    bool mRebuildComposite;
    float mAlphaMultiplier;
};


}