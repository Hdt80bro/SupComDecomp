#include "Set.h"
#include <algorithm>


// 0x004010E0
void Moho::BVIntSet::AddFrom(Moho::BVIntSet *from, unsigned int lower, unsigned int upper) {
    if (from == this || lower == upper) {
        return;
    }

    this->EnsureBounds(lower, upper);
    int offset = from->mStart - this->mStart;
    int k = this->BucketFor(lower);
    int end = this->BucketFor(upper);
    unsigned int beginShift = lower & 0x1F;
    unsigned int endShift = upper & 0x1F;
    if (k == end) {
        unsigned int mask = (0xFFFFFFFF << beginShift) & ((1 << endShift) - 1);
        if (mask != 0) {
            this->mUsed[k] |= from->mUsed[k - offset] & mask;
        }
    } else {
        this->mUsed[k] |= from->mUsed[k - offset] & (0xFFFFFFFF << beginShift);
        while (++k < end) {
            this->mUsed[k] |= from->mUsed[k - offset];
        }
        if (endShift != 0) {
            this->mUsed[end] |= from->mUsed[end - offset] & ((1 << endShift) - 1);
        }
    }
}

// 0x00401670
Moho::BVIntSetIndex Moho::BVIntSet::ClearRange(Moho::BVIntSetIndex lower, Moho::BVIntSetIndex upper) {
    int start = this->BucketFor(lower.mVal);
    int end = this->BucketFor(upper.mVal);
    unsigned int startMask = lower.mVal & 0x1F;
    unsigned int endMask = upper.mVal & 0x1F;
    if (start == end) {
        if (startMask != endMask) {
            this->mUsed[start] &= ((1 << startMask) - 1) | (0xFFFFFFFF << endMask);
        }
    } else {
        this->mUsed[start] &= (1 << startMask) - 1;
        for (int k = start + 1; k < end; ++k) {
            this->mUsed[k] = 0;
        }
        if (endMask != 0) {
            this->mUsed[end] &= 0xFFFFFFFF << endMask;
        }
    }
    this->Finalize();
    return Moho::BVIntSetIndex{this, std::min(upper.mVal, this->Max())};
}

// 0x004017B0
unsigned int Moho::BVIntSet::GetNext(unsigned int val) const {
    val = std::min(val + 1, this->Min());
    unsigned int bucket = std::min(this->BucketFor(val), this->Buckets());
    unsigned int used = this->mUsed[bucket] >> (val & 0x1F);
    while (used == 0) {
        if (++bucket >= this->Buckets()) {
            return this->Max();
        }
        used = this->mUsed[bucket];
        val = this->FromBucket(bucket);
    }
    while ((used & 1) == 0) {
        used >>= 1;
        ++val;
    }
    return val;
}

// 0x004018A0
void Moho::BVIntSet::Finalize() {
    int lower = 0;
    int upper = this->Buckets();
    if (upper != 0) {
        while (this->mUsed[lower] == 0) {
            if (++lower >= upper) {
                break;
            }
        }
        if (lower < upper) {
            while (this->mUsed[upper - 1] == 0) {
                --upper;
            }
        }
    }
    if (upper != this->Buckets()) {
        if (lower == upper) {
            this->mUsed.Clear();
            this->mStart = 0;
        } else {
            unsigned int *begin = &this->mUsed[lower];
            unsigned int *end = &this->mUsed[upper];
            size_t size = end - begin;
            if (size > 0) {
                memmove_s(this->mUsed.begin(), 4 * size, begin, 4 * size);
            }
            unsigned int fill = 0;
            this->mUsed.Resize(upper - lower, &fill);
            this->mStart += lower;
        }
    }
}

// 0x00401980
void Moho::BVIntSet::EnsureBounds(unsigned int lower, unsigned int upper) {
    unsigned int start = lower >> 5;
    unsigned int end = (upper + 31) >> 5;
    if (this->mUsed.Empty()) {
        if (start < end) {
            this->mStart = start;
            unsigned int fill = 0;
            this->mUsed.Resize(end - start, &fill);
        }
    } else {
        size_t oldSize = this->Buckets();
        int dec = 0, inc = 0;
        if (start < this->mStart) {
            dec = this->mStart - start;
        }
        if (end > this->mStart + oldSize) {
            inc = end - (this->mStart + oldSize);
        }
        if (dec != 0 || inc != 0) {
            size_t newSize = dec + inc + this->Buckets();
            unsigned int fill = 0;
            this->mUsed.Resize(newSize, &fill);
            if (dec != 0) {
                int blocks = (4 * oldSize) >> 2;
                if (blocks > 0) {
                    memmove_s(&this->mUsed[oldSize + dec - blocks], 4 * blocks, this->mUsed.begin(), 4 * blocks);
                }
                for (unsigned int *k = this->mUsed.begin(); k != &this->mUsed[dec]; ++k) {
                    *k = 0;
                }
                this->mStart = start;
            }
        }
    }
}

// 0x00401A60
void Moho::BVIntSet::AddAllFrom(Moho::BVIntSet *from) {
    this->AddFrom(from, from->GetNext(-1), from->Max());
}

// 0x004036A0
Moho::BVIntSetAddResult Moho::BVIntSet::Add(unsigned int val) {
    this->EnsureBounds(val, val + 1);
    unsigned int bucket = this->BucketFor(val);
    unsigned int old = this->mUsed[bucket];
    unsigned int shift = val & 0x1F;
    this->mUsed[bucket] = old | (1 << shift);
    return Moho::BVIntSetAddResult{this, val, ((old >> shift) & 1) == 0};
}
