#include "gpgcore/hastar/ClusterMap.h"

namespace Moho {

// 0x00E360D8
class OccupySourceBinding : public gpg::HaStar::IOccupationSource
{
public:
    void Func() override; // 0x0076B770
};

struct PathTables
{

    struct Impl
    {
        int mWidth;
        int mHeight;
        std::vector<gpg::HaStar::IOccupationSource> mSrcs;
        std::vector<gpg::HaStar::ClusterMap> mMaps;
        boost::shared_ptr<gpg::HaStar::ClusterCache> mClusterCache;
    };

    Moho::PathTables::Impl *mImpl;
};


}
