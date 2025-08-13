#include "gpgcore/hastar/ClusterMap.h"

namespace Moho {

struct PathTables
{

    struct Impl
    {
        int mWidth;
        int mHeight;
        std::vector<gpg::HaStar::IOccupationSource> mSrcs;
        std::vector<gpg::HaStar::ClusterMap> mMaps;
        boost::shared_ptr<Moho::RScmResource> mClusterCache;
    };

    Moho::PathTables::Impl *mImpl;
};


}
