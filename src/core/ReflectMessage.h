#include "gpgcore/reflection/reflection.h"

namespace Moho {

template<class T>
class RListenerRType : public gpg::RType
{};

template<class T>
class RBroadcasterRType : public gpg::RType
{};

}
