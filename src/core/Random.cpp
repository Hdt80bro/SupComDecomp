#include "Random.h"

void Moho::CMersenneTwister::Seed(unsigned int seed) {
    this->state[0] = seed;
    for (int k = 1; k < 624; ++k) {
        seed = k + 1812433253 * (seed ^ (seed >> 30));
        this->state[k] = seed;
    }
    this->k = 624;
    this->ShuffleState();
}

void Moho::CMersenneTwister::ShuffleState() {
    static unsigned int mersenne_twist_coef = {0, 0x9908B0DF};
    // MT19937
    for (int i = 0; i < 227; ++i) {
        int x = this->state[i] ^ (this->state[i] ^ this->state[i + 1]) & 0x7FFFFFFF;
        this->state[i] = this->state[i + 397] ^ mersenne_twist_coef[x & 1] ^ (x >> 1);
    }
    for ( ; i < 623; ++i) {
        int x = this->state[i] ^ (this->state[i] ^ this->state[i + 1]) & 0x7FFFFFFF;
        this->state[i] = this->state[i - 227] ^ mersenne_twist_coef[x & 1] ^ (x >> 1);
    }
    this->state[623] = ((this->state[623] ^ (this->state[623] ^ this->state[0]) & 0x7FFFFFFF) >> 1) ^ this->state[396] ^ mersenne_twist_coef[this->state[0] & 1];
    this->k = 0;
}
