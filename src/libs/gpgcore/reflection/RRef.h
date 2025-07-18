
namespace gpg {

class RType;

struct RRef {
    const char *name;
    gpg::RType *type;
    int offset;
    int v4;
    int desc;
};
    
}
