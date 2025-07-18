
namespace gpg {

class RType;

class RField {
    const char *name;
    gpg::RType *type;
    int offset;
    int v4;
    int desc;
};

}
