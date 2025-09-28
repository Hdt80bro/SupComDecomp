// known file

#include "reflection.h"
#include "gpgcore/General.h"
#include <algorithm>

// 0x008DF4A0
void gpg::RType::Finish() {
    GPG_ASSERT(!mInitFinished); // if (this->mInitFinished) { gpg::HandleAssertFailure("!mInitFinished", 165, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\reflection.cpp"); }
    std::sort(this->mFields.begin(), this->mFields.end());
}

// 0x008D8640
void gpg::RType::Version(int v) {
    GPG_ASSERT(mVersion==0 || mVersion==v); // if (this->mVersion != 0 && this->mVersion != v) { gpg::HandleAssertFailure("!mVersion==0 || mVersion==v", 146, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\reflection.cpp"); }
    this->mVersion = v;
}

// 0x008DF500
void gpg::RType::AddBase(const gpg::RField &field) {
    GPG_ASSERT(!mInitFinished); // if (this->mInitFinished) { gpg::HandleAssertFailure("!mInitFinished", 172, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\reflection.cpp"); }
    this->mBases.push_back(field);
    for (int i = 0; i < field.mType->mFields.size(); ++i) {
        gpg::RField &f = field.mType->mFields[i];
        this->mFields.push_back(gpg::RField{
            f.mName,
            f.mType,
            f.mOffset + field.mOffset,
            f.v4,
            f.mDesc
        });
    }
}

// 0x008D94E0
const gpg::RField *gpg::RType::GetFieldNamed(char const *name) const {
    GPG_ASSERT(!mInitFinished); // if (this->mInitFinished) { gpg::HandleAssertFailure("!mInitFinished", 185, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\reflection.cpp"); }
    int cur = 0;
    int end = this->mFields.size();
    while (cur < end) {
        int k = (end + cur) / 2;
        int cmp = strcmp(name, this->mFields[k].mName);
        if (cmp < 0) {
            end = k;
        } else if (cmp > 0) {
            cur = k + 1;
        } else {
            return &this->mFields[k];
        }
    }
    return nullptr;
}

// 0x008D8800
void gpg::RRef::Delete() {
    if (this->mObj != nullptr) {
        GPG_ASSERT(mType->mDelete); // if (this->mType->mDelete == nullptr) { gpg::HandleAssertFailure("mType->mDelete", 576, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\reflection\\reflection.cpp"); }
        this->mType->mDelete(this->mObj);
    }
}
