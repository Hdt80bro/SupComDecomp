#include "gpgcore/hastar/Cluster.h"
#include "gpgcore/containers/Array2D.h"
#include "gpgcore/containers/Rect.h"
#include "boost/shared_ptr.hpp"


struct struct_SubCluster
{
    gpg::HaStar::Cluster **vec;
    int width;
    int height;
};


namespace gpg::HaStar {

// 0x00E360D0
class IOccupationSource
{
public:
    virtual void Func() = 0;
};

struct ClusterMap
{
    int numlevels;
    int width;
    int height;
    gpg::HaStar::IOccupationSource *src;
    boost::shared_ptr<Moho::RScmResource> v4;
    struct_SubCluster v6[4];
    gpg::BitArray2D checkLevels[4];
    bool isDone;
    int progress;
    gpg::Rect2i v36;
};


}
