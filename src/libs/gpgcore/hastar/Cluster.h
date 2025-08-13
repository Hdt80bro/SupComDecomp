
namespace gpg::HaStar {

struct Cluster
{
    struct Data
    {
        int rrefs;
        int v1;
        int v2;
    };

    gpg::HaStar::Cluster::Data *data;
};


}
