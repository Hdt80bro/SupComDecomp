#include "gpgcore/reflection/RRef.h"

namespace gpg {

// 0x00DFFECC
class RIndexed
{
public:
    virtual void SubscriptIndex(int) const;
    virtual bool IsNonNull() const;
    virtual void SetCount(void *, int) const;
    virtual void AssignPointer(void *, const gpg::RRef &) const;

};

}
