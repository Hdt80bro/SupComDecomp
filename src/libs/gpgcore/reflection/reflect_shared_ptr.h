// known file

#include "gpgcore/reflection/reflection.h"

namespace gpg {

template<class T>
class RSharedPointerType : public gpg::RType, public gpg::RIndexed
{};

}
