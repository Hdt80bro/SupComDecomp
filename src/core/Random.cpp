#include "Random.h"

// 0x0040E9F0
unsigned int Moho::CMersenneTwister::IRand() {
    if (this->mPos >= 624) {
        this->ShuffleState();
    }
    unsigned int x = this->mState[this->mPos];
    ++this->mPos;
    unsigned int a = x >> 11;
    unsigned int b = a ^ x;
    unsigned int c = (b & 0xFF3A58AD) << 7;
    x ^= (((c ^ b) & 0xFFFFDF8C) << 15) ^ c ^ a;
    return x ^ (x >> 18);
}

// 0x0040EB60
void Moho::CMersenneTwister::Seed(unsigned int seed) {
    this->mState[0] = seed;
    for (int k = 1; k < 624; ++k) {
        seed = k + 0x6C078965 * (seed ^ (seed >> 30));
        this->mState[k] = seed;
    }
    this->mPos = 624;
    this->ShuffleState();
}

// 0x0040EBB0
void Moho::CMersenneTwister::ShuffleState() {
    static unsigned int mersenne_twist_coef[] = {0, 0x9908B0DF}; // 0x00F5C994
    // the two loops have been combined with modulo for brevity
    // MT19937
    for (int i = 0 ; i < 623; ++i) {
        int x = this->mState[i] ^ (this->mState[i] ^ this->mState[i + 1]) & 0x7FFFFFFF;
        this->mState[i] = this->mState[(i + 397) % 624] ^ mersenne_twist_coef[x & 1] ^ (x >> 1);
    }
    this->mState[623] = ((this->mState[623] ^ (this->mState[623] ^ this->mState[0]) & 0x7FFFFFFF) >> 1) ^ this->mState[396] ^ mersenne_twist_coef[this->mState[0] & 1];
    this->mPos = 0;
}

// 0x0040EC60
void Moho::CMersenneTwister::Checksum(gpg::MD5Context &cont) {
    cont.Update(this, sizeof(*this));
}

// 0x0040EA70
float Moho::CRandomStream::FRand() {
    return IRand() * 2.3283064e-10;
}

// 0x0040EEC0
float Moho::CRandomStream::FRandGaussian() {
    if (this->mHasMarsagliaPair) {
        this->mHasMarsagliaPair = false;
        return this->mMarsagliaPair;
    } else {
        float x, y, s;
        do {
            x = FRand(-1, 1);
            y = FRand(-1, 1);
            s = x*x + y*y;
        } while (s >= 1.0);
        s = sqrtf(-2.0 * log(s) / s);
        this->mHasMarsagliaPair = true;
        this->mMarsagliaPair = y * s;
        return x * s;
    }
}
