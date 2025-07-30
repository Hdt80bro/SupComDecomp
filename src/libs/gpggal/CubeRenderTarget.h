
namespace gpg::gal {

// 0x00D420AC
class CubeRenderTargetContext
{
public:
    int v1;
    int v2;

    ~CubeRenderTargetContext() = default; // 0x008E65A0
};

// 0x00D43000
class CubeRenderTarget
{
public:
    virtual ~CubeRenderTarget() = 0;
};

}
