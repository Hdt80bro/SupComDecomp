#include "gpgcore/reflection/RRef.h"

namespace gpg {

class RType;

// 0x00D4145C
class RObject {
public:
    virtual gpg::RType *GetClass() const = 0;
    virtual gpg::RRef GetDerivedObjectRef() = 0;
    virtual ~RObject() = default; // 0x004012D0
};

}
