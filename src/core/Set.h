#include "gpgcore/containers/fastvector.h"

namespace Moho {

struct BVIntSet;

struct BVIntSetIndex
{
    Moho::BVIntSet *mSet;
    unsigned int mVal;
};

struct BVIntSetAddResult : Moho::BVIntSetIndex
{
    bool mIsNew;
};

struct BVIntSet
{
    unsigned int mStart;
    unsigned int unk;
    gpg::fastvector_n<unsigned int, 2> mUsed;

    size_t Buckets() const {
        return this->mUsed.Size();
    }
    size_t BucketFor(unsigned int val) const {
        return (val >> 5) - this->mStart;
    }
    unsigned int FromBucket(size_t bucket) const {
        return (this->mStart + bucket) << 5;
    }
    unsigned int Min() const {
        return this->FromBucket(0);
    }
    unsigned int Max() const {
        return this->FromBucket(this->Buckets());
    }

    void AddFrom(Moho::BVIntSet *from, unsigned int lower, unsigned int upper); // 0x004010E0
    Moho::BVIntSetIndex ClearRange(Moho::BVIntSetIndex start, Moho::BVIntSetIndex end); // 0x00401670
    unsigned int GetNext(unsigned int val) const; // 0x004017B0
    void Finalize(); // 0x004018A0
    void EnsureBounds(unsigned int lower, unsigned int upper); // 0x00401980
    void AddAllFrom(Moho::BVIntSet *from); // 0x00401A60
    Moho::BVIntSetAddResult Add(unsigned int val); // 0x004036A0
};

}