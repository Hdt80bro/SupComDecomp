
namespace gpg {

class RType;

struct RField {
    const char *name;
    gpg::RType *type;
    int offset;
    int v4;
    int desc;
};

}
