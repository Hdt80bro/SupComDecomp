
namespace gpg {

class RType;

struct RField {
    const char *mName;
    gpg::RType *mType;
    int mOffset;
    int v4;
    int mDesc;
};

}
