// known file

#include "gpgcore/streams/ReadArchive.h"
#include "gpgcore/streams/WriteArchive.h"
#include "gpgcore/reflection/reflection.h"
#include "gpgcore/DListItem.h"
#include "boost/shared_ptr.hpp"

namespace gpg {
    

enum TrackedPointerState
{
    RESERVED = 0x0,
    UNOWNED = 0x1,
    OWNED = 0x2,
    SHARED = 0x3,
};


// 0x00D48B90
class SerHelperBase :
    public gpg::DListItem<gpg::SerHelperBase>
{
    virtual void Init() = 0;
};

template<class T>
class SerSaveLoadHelper : public gpg::SerHelperBase
{
public:
    using class_t = T;

public:
    void (*mDeserialize)(gpg::ReadArchive *arch, class_t *obj);
    void (*mSerialize)(gpg::WriteArchive *arch, class_t *obj);

    void Init() override {
        gpg::RType *type = T::StaticGetClass();
        GPG_ASSERT(!type->mSerLoadFunc); /* if (type->mSerLoadFunc) { gpg::HandleAssertFailure("!type->mSerLoadFunc", 84, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/serialization.h"); } */
        type->mSerLoadFunc = this->mDeserialize;

        GPG_ASSERT(!type->mSerSaveFunc); /* if (type->mSerSaveFunc) { gpg::HandleAssertFailure("!type->mSerSaveFunc", 87, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/serialization.h"); } */
        type->mSerSaveFunc = this->mSerialize;
    }

};

template<class T>
class SerConstructHelper : public gpg::SerHelperBase
{
public:
    void (*mConstruct)(void *);
    void (*mDeconstruct)(void *);
    
    void Init() override {
        gpg::RType *type = T::StaticGetClass();
        GPG_ASSERT(!type->mSerConstructFunc); /* if (type->mSerConstructFunc) { gpg::HandleAssertFailure("!type->mSerConstructFunc", 231, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/serialization.h"); } */
        type->mSerConstructFunc = this->mConstruct;
        type->mSerDeconstructFunc = this->mDeconstruct;
    }
};


template<class T>
class SerSaveConstructHelper : public gpg::SerHelperBase
{
public:
    using class_t = T;

public:
    void (*mConstruct)(gpg::WriteArchive *arch, class_t *obj);

    void Init() override {
        gpg::RType *type = T::StaticGetClass();
        GPG_ASSERT(!type->mSerSaveConstructArgsFunc); /* if (type->mSerSaveConstructArgsFunc) { gpg::HandleAssertFailure("!type->mSerSaveConstructArgsFunc", 189, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/serialization.h"); } */
        type->mSerSaveConstructArgsFunc = this->mSerialize;
    }

};

struct SerSaveConstructArgsResult
{
    gpg::TrackedPointerState mOwnership;
    bool mShared;

    void SetOwned(unsigned int a2); // 0x0094F750
    void SetUnowned(unsigned int a2); // 0x0094F790
    void SetShared(unsigned int a2); // 0x0094F7D0
};

struct SerConstructResult
{
    gpg::RRef mRef;
    boost::shared_ptr<void> mSharedPtr;
    gpg::TrackedPointerState mState;
    bool mShared;
    
    void SetOwned(const gpg::RRef &ptr, unsigned int a2); // 0x0094F5E0
    void SetUnowned(const gpg::RRef &ptr, unsigned int a2); // 0x0094F630
    void SetShared(const gpg::RRef &ptr, unsigned int a2); // 0x0094F680
    void SetShared(const boost::shared_ptr<void> &ptr, gpg::RType *type, unsigned int a2); // 0x0094F6D0
};


}
