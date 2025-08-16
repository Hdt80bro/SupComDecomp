// known file

#include "gpgcore/streams/WriteArchive.h"
#include "gpgcore/streams/ReadArchive.h"
#include <vector>

namespace gpg {

class REnumType;
class RType;
class RIndexed;

struct RRef {
    void *mObj;
    gpg::RType *mType;
};


void PreRegisterRType(type_info *info, gpg::RType *type); // 0x008DF850
gpg::RType *LookupRType(type_info *info); // 0x008E0750


// 0x00D4145C
class RObject {
public:
    virtual gpg::RType *GetClass() const = 0; // StaticGetClass sometimes
    virtual gpg::RRef GetDerivedObjectRef() = 0;
    virtual ~RObject() = default; // 0x004012D0
};

struct RField {
    const char *mName;
    gpg::RType *mType;
    int mOffset;
    int v4;
    int mDesc;

    RField(gpg::RType *type, const char *name, int offset);
};


// 0x00D41694
class RType : public gpg::RObject
{
public:
    bool mFinished;
    bool mInitFinished;
    int mSize;
    int mVersion;
    int mSerSaveConstructArgsFunc;
    void (*mSerSaveFunc)(gpg::WriteArchive *, int, int, int);
    int mSerConstructFunc;
    void (*mSerLoadFunc)(gpg::ReadArchive *, int, int, gpg::RRef *);
    int v8;
    int v9;
    std::vector<gpg::RField> mBases;
    std::vector<gpg::RField> mFields;
    gpg::RRef *(*mNewRef)(gpg::RRef *);
    int mCpyRef;
    void (*mSerDeconstructFunc)(void *);
    int mCtrRef;
    int mMovRef;
    int mDtr;
    bool v24;

    gpg::RType *GetClass() const override; // 0x00401370
    gpg::RRef GetDerivedObjectRef() override; // 0x00401390
    ~RType() override = default;
    virtual const char *GetName() = 0;
    virtual std::string GetLexical(const gpg::RRef &) const; // 0x008DB100
    virtual bool SetLexical(const gpg::RRef &, const char *) const; // 0x008D86E0
    virtual const gpg::RIndexed *IsIndexed() const; // 0x004013B0
    virtual const gpg::RIndexed *IsPointer() const; // 0x004013C0
    virtual const gpg::REnumType *IsEnumType() const; // 0x004013D0
    virtual void Init(); // 0x008D8680
    virtual void Finish(); // 0x008DF4A0

    void RegisterType(); // 0x008DF960

    template<class T>
    gpg::RField *AddField(const char *name, int offset) {
        GPG_ASSERT(!mInitFinished); // if (this->mInitFinished) { gpg::HandleAssertFailure("!mInitFinished", 734, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/reflection.h"); }
        gpg::RField f{T::StaticGetClass(), name, offset};
        this->mFields.push_back(f);
        return this->mFields.back();
    }
};

// 0x00DFFECC
class RIndexed
{
public:
    virtual void SubscriptIndex(int) const = 0;
    virtual bool IsNonNull() const = 0;
    virtual void SetCount(void *, int) const; // 0x004012F0
    virtual void AssignPointer(void *, const gpg::RRef &) const; // 0x00401320

};

struct struct_EnumValue
{
    int mValue;
    const char *mName;
};

// 0x00D48CA0
class REnumType : public gpg::RType
{
public:
    const char *mPrefix;
    std::vector<struct_EnumValue> mEnumNames;
};

template<class T>
class RPointerType :
    public gpg::RType,
    public gpg::RIndexed
{
public:
    static RPointerType<T> *sPointerType = nulltpr;

    static RPointerType<T> GetType() {
        static RPointerType<T> PointerType{};
        if (sPointerType == nullptr) {
            sPointerType = gpg::LookupRType(typeid(T*));
        }
        return sPointerType;
    }

public:
    const char *GetName() override {

    }
    std::string GetLexical(const gpg::RRef &) const override {

    }
    gpg::RIndexed *IsIndexed() const override {
        return static_cast<gpg::RIndexed*>(this);
    }
    gpg::RIndexed *IsPointer() const override {
        return static_cast<gpg::RIndexed*>(this);
    } 
    void Init() override {

    }
    void SubscriptIndex(int) override {

    }
    bool IsNonNull() const override {

    }
    void AssignPointer(void *void_pptr, const gpg::RRef &ref) const override {
        GPG_ASSERT(void_pptr); // if (void_pptr == nullptr) { gpg::HandleAssertFailure("void_pptr", 663, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/reflection.h"); }
        gpg::RType *type = T::StaticGetClass();
        void *up = gpg::REF_UpcastPtr(ref, type)->mObj;
        if (ref->mObj != nullptr && up == nullptr) {
            throw gpg::BadRefCast{"type error"};
        }
        *void_pptr = up;
    }

    RPointerType() {
        gpg::PreRegisterRType(typeid(T*), this);
    }

};


template<class T>
class RListType : public gpg::RType
{};

template<class K, class V>
class RMapType : public gpg::RType
{};

template<class T>
class RVectorType : public gpg::RType, public gpg::RIndexed
{};

template<class T>
class RFastVectorType : public gpg::RType, public gpg::RIndexed
{};

template<class T>
class RListenerRType : public gpg::RType
{};

template<class T>
class RBroadcasterRType : public gpg::RType
{};

template<class T>
class RWeakPtrType : public gpg::RType
{};

}
