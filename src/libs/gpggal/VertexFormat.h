#include <vector>

namespace gpg::gal {

// 0x00D449A4
class VertexFormat
{
public:
    int format;
    std::vector<int> v2;

    virtual ~VertexFormat() = 0;
};

}
