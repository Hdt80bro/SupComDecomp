
namespace gpg::HaStar {

struct Cluster
{
    struct Node
    {
        short mVal;
    };

    struct Edge
    {
        char mVal;
    };

    // variable size!
    struct Data
    {
        int mRefs;
        int v1;
        int v2;
        char mNodeCount;
        //! gpg::HaStar::Cluster::Node mNodes[mNodeCount];
        //! gpg::HaStar::Cluster::Edge mEdges[(mNodeCount * (mNodeCount - 1)) / 2];
    };

    gpg::HaStar::Cluster::Data *mData;
};


}
