#include <vector>

namespace gpg::gal {

// 0x00D449A4
class VertexFormat
{
public:
    int mFormat;
    std::vector<int> mVec;

    virtual ~VertexFormat() = 0;
};

}
