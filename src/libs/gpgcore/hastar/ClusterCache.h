#include "gpgcore/hastar/Cluster.h"

namespace gpg::HaStar {

struct OccupationData
{
    DWORD v1;
    DWORD v2;
    DWORD v3;
    DWORD v4;
    DWORD v5;
    DWORD v6;
    DWORD v7;
    DWORD v8;
    DWORD v9;
    DWORD v10;
};


struct SubclusterData
{
    gpg::HaStar::Cluster mClusters[16];
    int mLevel;
    int mVal;
};


template<class T>
class ClusterInternalCache : public gpg::HaStar::Cluster::ICache
{
public:
    T mVec;

    int Func(void *) override {

    }
};

// 0x00D47884
template<>
class ClusterInternalCache<gpg::HaStar::OccupationData>
{};

// 0x00D4788C
template<>
class ClusterInternalCache<gpg::HaStar::SubclusterData>
{};

struct ClusterCache
{
    gpg::HaStar::ClusterInternalCache<gpg::HaStar::OccupationData> mOccupationData;
    gpg::HaStar::ClusterInternalCache<gpg::HaStar::SubclusterData> mSubclusterData;
};

}
