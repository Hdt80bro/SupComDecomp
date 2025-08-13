
namespace gpg::gal {

// 0x00D47C78
class IndexBufferContext
{
public:
    int mFormat;
    int mSize;
    int mType;

    virtual ~IndexBufferContext() = default; // 0x00940640
};

// 0x00D449A4
class IndexBuffer
{
public:

    virtual ~IndexBuffer() = 0;
    virtual gpg::gal::IndexBufferContext *GetContextBuffer() = 0;
};

}
