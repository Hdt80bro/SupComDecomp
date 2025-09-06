#include "gpgcore/algorithms/AStarSearch.h"

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
        char mNodeCount; // note: there is no alignment and `mNodes` begins at offset 0xD
        //! gpg::HaStar::Cluster::Node mNodes[mNodeCount];
        //! gpg::HaStar::Cluster::Edge mEdges[(mNodeCount * (mNodeCount - 1)) / 2];

        gpg::HaStar::Cluster::Node *Nodes() {
            return (gpg::HaStar::Cluster::Node *) (&this->mNodeCount + 1);
        }
        gpg::HaStar::Cluster::Edge *Edges() {
            return (gpg::HaStar::Cluster::Edge *) &this->Nodes()[this->mNodeCount];
        }
        void *operator new(size_t count, size_t nnodes) {
            return new char[count * (sizeof(gpg::HaStar::Cluster::Data) + 2 * nnodes + TriangularSize(nnodes))];
        }
    };

    class ICache
    {
    public:
        virtual int Func(void *) = 0;
    };

    gpg::HaStar::Cluster::Data *mData;

    void SetData(const gpg::HaStar::Cluster::Node *, const gpg::HaStar::Cluster::Edge *, unsigned int); // 0x00954110
};

unsigned int hash_value(const gpg::HaStar::Cluster &); // 0x009540E0

inline size_t TriangularSize(int n) {
    return (n * (n - 1)) / 2;
}

}
