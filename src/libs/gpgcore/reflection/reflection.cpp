// known file

#include "reflection.h"

void gpg::RType::Finish() {
    GPG_ASSERT(!mInitFinished); // if (this->mInitFinished) { gpg::HandleAssertFailure("!mInitFinished", 165, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\reflection.cpp"); }
    std::sort(this->mFields.begin(), this->mFields.end());
}
