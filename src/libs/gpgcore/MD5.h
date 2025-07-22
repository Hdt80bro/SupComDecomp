#include <string>
#include <bit>

namespace gpg {

struct MD5Digest
{
    int vals[4];

    bool operator!=(gpg::MD5Digest *); // 0x0073B100
    std::string ToString() const; // 0x008E5910
};

// synthetic helper not found in symbol tables
struct MD5Block
{
    char vals[56];
    __int64 size;
};

struct MD5Context
{
    gpg::MD5Digest digest;
    gpg::MD5Block block;
    size_t pos, gap;
    __int64 size;

    void Update(std::string); // 0x004A48B0
    void Reset(); // 0x008E4F40
    int RoundFunc1(int b, int c, int d) const {
        return b & c | d & ~b;
    } // 0x008E4F70
    int Round1(int v, int a, int b, int c, int d, int h, char s) const {
        return b + std::rotl(v + a + RoundFunc1(b, c, d) + h, s);
    } // 0x008E4F80
    int RoundFunc2(int b, int c, int d) const {
        return b & d | c & ~d;
    } // 0x008E4FB0
    int Round2(int v, int a, int b, int c, int d, int h, char s) const {
        return b + std::rotl(v + a + RoundFunc2(b, c, d) + h, s);
    } // 0x008EFC0
    int RoundFunc3(int b, int c, int d) const {
        return b ^ c ^ d;
    } // 0x008E4FF0
    int Round3(int v, int a, int b, int c, int d, int h, char s) const {
        return b + std::rotl(v + a + RoundFunc3(b, c, d) + h, s);
    } // 0x008E5000
    int RoundFunc4(int b, int c, int d) const {
        return c ^ (b | ~d);
    } // 0x008E5020
    int Round4(int v, int a, int b, int c, int d, int h, char s) const {
        return b + std::rotl(v + a + RoundFunc4(b, c, d) + h, s);
    } // 0x008E5030


    void ProcessBlock(gpg::MD5Digest &, const void *) const; // 0x008E5050
    gpg::MD5Digest Digest(); // 0x008E5790
    void Update(const void *, size_t); // 0x008E5870
};

}
