#include "gpgcore/MD5.h"

namespace Moho {

struct CMersenneTwister
{
    unsigned int state[624];
    int k;

    unsigned int IRand(); // 0x0040E9F0
    void Seed(unsigned int); // 0x0040EB60
    void ShuffleState(); // 0x0040EBB0
    void Checksum(gpg::MD5Context &); // 0x0040EC60
};

struct CRandomStream : Moho::CMersenneTwister
{
    float marsagliaPair;
    bool hasMarsagliaPair;

    float FRand(); // 0x0040EA70
    float FRandGaussian(); // 0x0040EEC0
    float FRand(float scale) {
        return FRand() * scale;
    } // inline
    float FRand(float min, float max) {
        return min + FRand(max - min);
    } // inline
    double DRand() {
        return ((double) (IRand() >> 6) + (double) (IRand() >> 5) * 67108864.0) * 1.110223024625157e-16;
    } // inline
    unsigned int IRand(int scale) {
        return (scale * (__int64) IRand()) >> 32;
    } // inline
    float IRand(unsigned int min, unsigned int max) {
        return min + IRand(max - min);
    } // inline
};

}
