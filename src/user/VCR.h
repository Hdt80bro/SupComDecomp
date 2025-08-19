#include "user/WldSession.h"
#include "gpgcore/streams/Stream.h"
#include "gpgcore/String.h"
#include <memory>

namespace Moho {

std::auto_ptr<gpg::Stream> VCR_CreateReplay(const Moho::SWldSessionInfo *, gpg::StrArg); // 0x00875B60
std::auto_ptr<Moho::SWldSessionInfo> VCR_SetupReplaySession(gpg::StrArg); // 0x008765E0

}
