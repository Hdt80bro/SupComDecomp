#include "gpgcore/hastar/ClusterMap.h"

namespace Moho {

struct PathTables
{

    struct Impl
    {
        int width;
        int height;
        std::vector<gpg::HaStar::IOccupationSource> srcs;
        std::vector<gpg::HaStar::ClusterMap> maps;
        boost::shared_ptr<Moho::RScmResource> clusterCache;
    };

    Moho::PathTables::Impl *impl;
};


}
