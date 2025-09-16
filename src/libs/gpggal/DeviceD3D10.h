#include "gpggal/Device.h"

namespace gpg::gal {

// 0x00D4340C
class DeviceD3D10 : public gpg::gal::Device
{
public:
    HMODULE mD3DModule1;
    HMODULE mD3DModule2;
    HMODULE mDXGIModule;
    HRESULT (__cdecl *mCreateDevice)(IDXGIAdapter *pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, ID3D10Device **ppDevice);
    DWORD mCreateBlob;
    HRESULT (__cdecl *mCreateEffectFromMemory)(LPCVOID pData, SIZE_T DataLength, LPCSTR pSrcFileName, const D3D10_SHADER_MACRO *pDefines, ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice, ID3D10EffectPool *pEffectPool, ID3DX10ThreadPump *pPump, ID3D10Effect **ppEffect, ID3D10Blob **ppErrors, HRESULT *pHResult);
    HRESULT (__thiscall *mCreateTextureFromMemory)(ID3D10Device *pDevice, LPCVOID pSrcData, SIZE_T SrcDataSize, D3DX10_IMAGE_LOAD_INFO *pLoadInfo, ID3DX10ThreadPump *pPump, ID3D10Resource **ppTexture, HRESULT *pHResult);
    HRESULT (__stdcall *mSaveTextureToFileA)(ID3D10Resource *pSrcTexture, D3DX10_IMAGE_FILE_FORMAT DestFormat, LPCSTR pDestFile);
    HRESULT (__cdecl *mSaveTextureToMemory)(ID3D10Resource *pSrcTexture, D3DX10_IMAGE_FILE_FORMAT DestFormat, LPD3D10BLOB *ppDestBuf, UINT Flags);
    HRESULT (__cdecl *mCreateDXGIFactory)(REFIID riid, IDXGIFactory **ppFactory);
    DWORD v10;
    std::vector<std::string> mStrs;
    gpg::gal::DeviceContext mDeviceContext;
    std::vector<AdapterD3D10> mAdapters;
    std::vector<IDXGISwapChain> mSwapChains;
    boost::shared_ptr<PipelineStateD3D10> mPipelineState;
    IDXGIFactory *mDXGIFactory;
    ID3D10Device *mDevice;
    ID3D10Effect *mEffectPreamble;
    ID3D10Effect *mShaderPreamble;
    ID3D10EffectTechnique *mTechnique_RTT;
    ID3D10Buffer *mBuffer;
    ID3D10InputLayout *mInputLayout;
    DWORD v45;
    DWORD v46;
    DWORD v47;
    DWORD v48;
    DWORD v49;
    DWORD v50;
    DWORD v51;
    DWORD v52;
    DWORD v53;
    DWORD v54;
    DWORD v55;
    DWORD v56;
    DWORD v57;
    DWORD v58;
    DWORD v59;
    DWORD v60;
    gpg::gal::OutputContext *mOutputContexts;
    gpg::gal::CursorD3D10 mCursor;
};

}
