
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


template<class T>
class ClusterInternalCache
{
public:
    T vec;

    virtual void Func();
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
    gpg::HaStar::ClusterInternalCache<gpg::HaStar::OccupationData> occupationData;
    gpg::HaStar::ClusterInternalCache<gpg::HaStar::SubclusterData> subclusterData;
};

}