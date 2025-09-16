#include "core/D3DRes.h"
#include "core/DiskWatch.h"
#include "gpggal/Device.h"

namespace Moho {

// 0x00E02714
class ID3DDeviceResources
{
public:
    virtual void Func0() = 0;
    virtual void Func1() = 0;
    virtual void CreateRednerTarget() = 0;
    virtual void Func3() = 0;
    virtual void CreateDeptchStencil() = 0;
    virtual void GetVertexFormat() = 0;
    virtual void NewVertexSheet() = 0;
    virtual void CreateIndexSheet() = 0;
    virtual void Func5() = 0;
    virtual void Func6() = 0;
    virtual void GetTexture() = 0;
    virtual void LoadPrefetchData() = 0;
    virtual void Func7() = 0;
    virtual void GetTextureSheet() = 0;
    virtual void CreateDynamicTextureSheet2() = 0;
    virtual void NewDynamicTextureSheet() = 0;
    virtual void CreateDynamicTextureSheet1() = 0;
    virtual void Func9() = 0;
    virtual void Func10() = 0;
    virtual void FindEffect() = 0;
    virtual void Func11() = 0;
};

class CD3DDeviceResources :
    public Moho::ID3DDeviceResources, // 0x00E02864
    public Moho::CDiskWatchListener   // 0x00E028BC
{
public:

};

// 0x00E02214
class CD3DDevice
{
public:

    virtual ~CD3DDevice() = default; // 0x0042DBE0
    virtual gpg::gal::Device *GetDevice(); // 0x0042DBF0
    virtual void SetRenViewport(Moho::WRenViewport *viewport); // 0x0042DC10
    virtual Moho::WRenViewport *GetViewport(); // 0x0042E9D0
    virtual bool Refresh(); // 0x0042E9E0
    virtual int GetHeadWidth(unsigned int head); // 0x0042EA00
    virtual int GetHeadHeight(unsigned int head); // 0x0042EA00
    virtual Wm3::Vector2i GetSize(unsigned int head); // 0x0042EA60
    virtual double GetAspectRatio(unsigned int head); // 0x0042EA60
    virtual bool SetCursor(boost::shared_ptr<void>, boost::shared_ptr<void>); // 0x0042EB40
    virtual bool ShowCursor(bool show); // 0x0042EB00
    virtual void GetView(__out Wm3::Vector2i &pos, __out Wm3::Vector2i &size, __out float &minZ, __out float &maxZ); // 0x
    virtual void SetViewport();
    virtual void GetResources();
    virtual void Func9();
    virtual void Func10();
    virtual void Func11();
    virtual void Func12();
    virtual void SetColorWriteState();
    virtual void SetCurEffect();
    virtual void SelectFxFile();
    virtual void SelectTechnique();
    virtual void GetCurEffect();
    virtual void GetWriterLock1();
    virtual void GetWrtierLock2();
    virtual void Func16();
    virtual void Func17();
    virtual void GetRenderTarget();
    virtual void GetDepthStencil();
    virtual void BeginScene1();
    virtual void BeginScene2();
    virtual void BeginScene();
    virtual void SetRenderTarget1();
    virtual void SetRenderTarget2();
    virtual void EndScene();
    virtual void InitRenderEngineStats();
    virtual void AddPrimStats();
    virtual void AddVertexStats();
    virtual void AddQuadBatchStats();
    virtual void AddTextBatchStats();
    virtual void SetAntiAliasingSamples();
    virtual void Init();
    virtual void InitContext();
    virtual void Destroy();
    virtual void GetCurThreadId();
    virtual void UpdateSurface2();
    virtual void UpdateSurface();
    virtual void SetViewRect2();
    virtual void SetViewRect();
    virtual void SetViewRenderTarget2();
    virtual void SetViewRenderTarget();
    virtual void Clear2();
    virtual void Clear();
};

}
