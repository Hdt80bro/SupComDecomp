#include <vector>
#include "gpgcore/hastar/ClusterMap.h"

namespace Moho {

struct PathQueue
{
    struct ImplBase
    {
        int v1;
        int v2;
        int v3;
        std::vector<gpg::HaStar::ClusterMap> mClusters;
        int v8;
        int v9;
        int v10;
        int v11;
        int v12;
        int v13;
        int v14;
        int v16;
        int v17;
        int v18;
        int v19;
        int v20;
        int v21;
        int v22;
        int v23;
        int v24;
        int v25;
        int mBudget;
        int v26;
        int v27;
        int v28;
        int v29;
        int v30;
        int v31;
    };
    struct Impl
    {
        int mSize;
        int mHeight;
        int gap;
        Moho::PathQueue::ImplBase mBase;
    };


    Moho::PathQueue::Impl *mImpl;
};

}
