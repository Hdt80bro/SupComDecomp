#include "gpgcore/hastar/Cluster.h"
#include "gpgcore/hastar/ClusterCache.h"
#include "gpgcore/containers/Array2D.h"
#include "gpgcore/containers/Rect.h"
#include "boost/shared_ptr.hpp"


#define MAX_LEVEL 3

struct struct_Subcluster
{
    gpg::HaStar::Cluster *mArray;
    int mWidth;
    int mHeight;
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
    int mNumLevels;
    int mWidth;
    int mHeight;
    gpg::HaStar::IOccupationSource *mSrc;
    boost::shared_ptr<gpg::HaStar::ClusterCache> mCache;
    struct_Subcluster mLevel[4];
    gpg::BitArray2D mCheckLevels[4];
    bool mIsDone;
    int mProgress;
    gpg::Rect2i mArea;
};


}
