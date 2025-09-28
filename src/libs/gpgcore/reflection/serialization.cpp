// known file

#include "gpgcore/reflection/serialization.h"



// 0x0094F680
void gpg::SerConstructResult::SetOwned(const gpg::RRef &ptr, unsigned int a2) {
    GPG_ASSERT(mState == RESERVED); // if (this->mState != RESERVED) { gpg::HandleAssertFailure("mInfo.mState == RESERVED", 196, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mRef = ptr;
    this->mState = OWNED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}

// 0x0094F630
void gpg::SerConstructResult::SetUnowned(const gpg::RRef &ptr, unsigned int a2) {
    GPG_ASSERT(mState == RESERVED); // if (this->mState != RESERVED) { gpg::HandleAssertFailure("mInfo.mState == RESERVED", 196, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mRef = ptr;
    this->mState = UNOWNED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}

// 0x0094F680
void gpg::SerConstructResult::SetShared(const gpg::RRef &ptr, unsigned int a2) {
    GPG_ASSERT(mState == RESERVED); // if (this->mState != RESERVED) { gpg::HandleAssertFailure("mInfo.mState == RESERVED", 212, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mRef = ptr;
    this->mState = SHARED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}

// 0x0094F6D0
void gpg::SerConstructResult::SetShared(const boost::shared_ptr<void> &ptr, gpg::RType *type, unsigned int a2) {
    GPG_ASSERT(mState == RESERVED); // if (this->mState != RESERVED) { gpg::HandleAssertFailure("mInfo.mState == RESERVED", 220, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mSharedPtr = ptr;
    this->mRef = {ptr.get(), type};
    this->mState = SHARED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}



// 0x0094F750
void gpg::SerSaveConstructArgsResult::SetOwned(unsigned int a2) {
    GPG_ASSERT(mOwnership == RESERVED); // if (this->mOwnership != RESERVED) { gpg::HandleAssertFailure("mOwnership == RESERVED", 402, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mOwnership = OWNED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}

// 0x0094F790
void gpg::SerSaveConstructArgsResult::SetUnowned(unsigned int a2) {
    GPG_ASSERT(mOwnership == RESERVED); // if (this->mOwnership != RESERVED) { gpg::HandleAssertFailure("mOwnership == RESERVED", 409, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mOwnership = UNOWNED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}

// 0x0094F7D0
void gpg::SerSaveConstructArgsResult::SetShared(unsigned int a2) {
    GPG_ASSERT(mOwnership == RESERVED); // if (this->mOwnership != RESERVED) { gpg::HandleAssertFailure("mOwnership == RESERVED", 416, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\serialization.cpp"); }
    this->mOwnership = SHARED;
    if ((a2 & 1) != 0) {
        this->mShared = false;
    }
}

