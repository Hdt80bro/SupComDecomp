#include <vector>
#include "gpggal/Head.h"
#include "gpggal/CursorContext.h"
#include "gpggal/DrawIndexedContext.h"
#include "gpggal/IndexBuffer.h"
#include "gpggal/OutputContext.h"
#include "gpggal/PipelineState.h"
#include "gpggal/Texture.h"
#include "gpggal/VertexBuffer.h"
#include "gpggal/VertexFormat.h"


namespace gpg::gal {

// 0x00D420B4
class DeviceContext
{
public:
    int mDeviceType;
    bool v16;
    UINT mAdapter;
    bool mVSync;
    bool mHWBasedInstancing;
    bool mSupportsFloat16;
    int mVertexShaderProfile;
    int mPixelShaderProfile;
    int mMaxPrimitiveCount;
    int mMaxVertexCount;
    std::vector<gpg::gal::Head> mHeads;

    virtual ~DeviceContext() = default; // 0x00430570

    int GetHeadCount(); // 0x008E66E0
    gpg::gal::Head *GetHead(unsigned int idx); // 0x008E69C0
};


// 0x00D42224
class Device
{
public:
    static bool IsReady(); // 0x008E6720
    static gpg::gal::Device *GetInstance(); // 0x008E6730

public:
    gpg::gal::OutputContext mOutputContext;

    virtual ~Device() = 0;
    virtual std::vector<std::string> *GetLog() = 0;
    virtual gpg::gal::DeviceContext *GetDeviceContext() = 0;
    virtual int GetCurThreadId() = 0;
    virtual void Func1() = 0;
    virtual void GetModesForAdapter(std::vector<struct_AdapterMode> *modes, int adpter) = 0;
    virtual void GetHead1() = 0;
    virtual void GetHead2() = 0;
    virtual void GetPipelineState(boost::shared_ptr<gpg::gal::PipelineState> *) = 0;
    virtual void CreateEffect() = 0;
    virtual boost::shared_ptr<gpg::gal::Texture> CreateTexture(gpg::gal::TextureContext *) = 0;
    virtual void CreateVolumeTexture() = 0;
    virtual void CreateCubeRenderTarget() = 0;
    virtual void CreateDepthStencilTarget() = 0;
    virtual boost::shared_ptr<gpg::gal::VertexFormat> *CreateVertexFormat(int) = 0;
    virtual boost::shared_ptr<gpg::gal::VertexBuffer> *CreateVertexBuffer(gpg::gal::VertexBufferContext * = 0);
    virtual boost::shared_ptr<gpg::gal::IndexBuffer> *CreateIndexBuffer(gpg::gal::IndexBufferContext *) = 0;
    virtual void CreateRenderTarget(gpg::gal::D3DSurface **, boost::shared_ptr<gpg::gal::Texture> *a) = 0;
    virtual void StretchRect() = 0;
    virtual void UpdateSurface() = 0;
    virtual void Func3() = 0;
    virtual void Func4() = 0;
    virtual void Func5() = 0;
    virtual void Func6() = 0;
    virtual void Func7() = 0;
    virtual void Func8() = 0;
    virtual void Func9(gpg::gal::DeviceContext *) = 0;
    virtual int TestCooperativeLevel() = 0;
    virtual void BeginScene() = 0;
    virtual void EndScene() = 0;
    virtual void Present() = 0;
    virtual void SetCursor(gpg::gal::CursorContext *) = 0;
    virtual void InitCursor() = 0;
    virtual BOOL ShowCursor(BOOL) = 0;
    virtual void SetViewport(const D3DVIEWPORT9 *) = 0;
    virtual void GetViewport(D3DVIEWPORT9 *) = 0;
    virtual void ClearTarget(gpg::gal::OutputContext &); // 0x008E6940
    virtual gpg::gal::OutputContext GetContext(); // 0x008E6810
    virtual void Clear(char clearTarget, char clearZbuffer, char clearStencil, int color, float Z, int stencil) = 0;
    virtual int ClearTextures(IDirect3DSurface9 *) = 0;
    virtual void SetVertexDeclaration(boost::shared_ptr<gpg::gal::VertexFormat>) = 0;
    virtual void Func15(unsigned int) = 0;
    virtual void SetBufferIndices(boost::shared_ptr<gpg::gal::IndexBuffer>) = 0;
    virtual void SetFogState(char enable, D3DMATRIX *projection, int fogStart, int fogEnd, int fogColor) = 0;
    virtual void SetWireframeState(bool) = 0;
    virtual void SetColorWriteState(int, int) = 0;
    virtual void DrawIndexedPrimitive(gpg::gal::DrawIndexedContext *) = 0;
    virtual void DrawPrimitive() = 0;
    virtual void BeginTechnique() = 0;
    virtual void EndTechnique() = 0;
};

}


static gpg::gal::Device *sDeviceD3D; // 0x00F8E284
