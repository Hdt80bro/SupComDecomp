// known file

#include "gpgcore/streams/WriteArchive.h"
#include "gpgcore/streams/ReadArchive.h"
#include <typeinfo>
#include <vector>


namespace gpg {


class RIndexed;
class RType;
class REnumType;


void PreRegisterRType(type_info *info, gpg::RType *type); // 0x008DF850
gpg::RType *LookupRType(const type_info *info); // 0x008E0750

gpg::RRef REF_UpcastPtr(const gpg::RRef &ref, const gpg::RType *type); // 0x008D9590
const gpg::RType *REF_GetTypeIndexed(int ind); // 0x008DD940
const gpg::RType *REF_FindTypeNamed(const char *name); // 0x008DF910
size_t REF_GetTypeCount(); // 0x008DF950
void REF_RegisterAllTypes(); // 0x008E0810


struct RRef {
    void *mObj;
    gpg::RType *mType;

    std::string GetLexical() const; // 0x004A35D0
    bool SetLexical(const char *) const; // 0x004A3600
    const char *GetTypeName() const; // gpgcore.dll
    gpg::RRef operator[](unsigned int ind) const; // 0x004A3610
    size_t GetCount() const; // 0x004A3630
    const gpg::RType *GetRType() const; // 0x004A3650
    const gpg::RIndexed *IsIndexed() const; // 0x004A3660
    const gpg::RIndexed *IsPointer() const; // 0x004CC9E0
    int GetNumBases() const; // gpgcore.dll
    gpg::RRef GetBase(int ind) const; // gpgcore.dll
    int GetNumFields() const; // 0x004CC9B0
    gpg::RRef GetField(int ind) const; // gpgcore.dll
    const char *GetFieldName(int ind) const; // gpgcore.dll
    void Delete(); // 0x008D8800
};

// 0x00D4145C
class RObject {
public:
    virtual gpg::RType *GetClass() const = 0;
    virtual gpg::RRef GetDerivedObjectRef() = 0;
    virtual ~RObject() = default; // 0x004012D0

    bool IsA(gpg::RType *) const; // 0x
};

#define DEFINE_ROBJECT_COMMON(clazz)\
gpg::RType *clazz::StaticGetClass() {\
    static gpg::RType *sType;\
    if (sType == nullptr) {\
        sType = gpg::LookupRType(&typeid(clazz));\
    }\
    return sType;\
}\
gpg::RType *clazz::GetClass() const {\
    return clazz::StaticGetClass();\
}\
gpg::RRef clazz::GetDerivedObjectRef() {\
    return gpg::RRef{this, this->GetClass()};\
}

struct RField {
    const char *mName;
    gpg::RType *mType;
    int mOffset;
    int v4;
    const char *mDesc;

    RField(const char *name, gpg::RType *type, int offset);
    RField(const char *name, gpg::RType *type, int offset, int v, const char *desc);
};

// 0x00DFFECC
class RIndexed
{
public:
    virtual gpg::RRef SubscriptIndex(void *obj, int ind) const = 0;
    virtual size_t GetCount(void *obj) const = 0;
    virtual void SetCount(void *obj, int count) const { throw std::bad_cast{}; } // 0x004012F0
    virtual void AssignPointer(void *obj, const gpg::RRef &from) const { throw std::bad_cast{}; }; // 0x00401320
};




////////////////////////////////////////////////////////////////////////////////
///  Type
////////////////////////////////////////////////////////////////////////////////

// 0x00D41694
class RType : public gpg::RObject
{
public:
    using save_construct_args_func_t = void (*)(void *);
    using save_func_t =  void (*)(gpg::WriteArchive *, int, int, gpg::RRef *);
    using construct_func_t = void (*)(void *);
    using load_func_t = void (*)(gpg::ReadArchive *, int, int, gpg::RRef *);
    using new_ref_func_t = gpg::RRef (*)();
    using cpy_ref_func_t = gpg::RRef (*)(gpg::RRef *);
    using delete_func_t = void (*)(void *);
    using ctr_ref_func_t = gpg::RRef (*)(void *);
    using mov_ref_func_t = gpg::RRef (*)(void *, gpg::RRef *);
    using dtr_func_t = void (*)(void *);

public:
    bool mFinished;
    bool mInitFinished;
    int mSize;
    int mVersion;
    save_construct_args_func_t mSerSaveConstructArgsFunc;
    save_func_t mSerSaveFunc;
    construct_func_t mSerConstructFunc;
    load_func_t mSerLoadFunc;
    int v8;
    int v9;
    std::vector<gpg::RField> mBases;
    std::vector<gpg::RField> mFields;
    new_ref_func_t mNewRef;
    cpy_ref_func_t mCpyRef;
    delete_func_t mDelete;
    ctr_ref_func_t mCtrRef;
    mov_ref_func_t mMovRef;
    dtr_func_t mDtr;
    bool v24;

    static gpg::RType *StaticGetClass(); // 0x00401350
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

    void Version(int); // 0x008D8640
    void AddBase(const gpg::RField &); // 0x008DF500
    void RegisterType(); // 0x008DF960
    const gpg::RField *GetFieldNamed(const char *) const; // 0x008D94E0
    bool IsDerivedFrom(const gpg::RType *, int *) const; // 0x008DBFF0

    template<class T>
    gpg::RField *AddField(const char *name, int offset) {
        GPG_ASSERT(!mInitFinished); // if (this->mInitFinished) { gpg::HandleAssertFailure("!mInitFinished", 734, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/reflection.h"); }
        gpg::RField f{T::StaticGetClass(), name, offset};
        this->mFields.push_back(f);
        return this->mFields.back();
    }

    template<class T, class B>
    void AddBase() {
        gpg::RType *type = B::StaticGetClass();
        this->AddBase(gpg::RField{
            type->GetName(),
            type,
            offsetof(T, B) // !
        });
    }
};

#define DECL_REFLECT_TYPE(clazz) DECL_REFLECT_TYPE_FOR(clazz##TypeInfo, clazz)
#define DECL_REFLECT_TYPE_FOR(clazz, forClazz)\
class clazz : public gpg::RType\
{\
public:\
    using type = forClazz;\
\
    ~clazz() override = default;\
    const char *GetName() override {\
        return #forClazz;\
    };\
    void Init() override;\
}




////////////////////////////////////////////////////////////////////////////////
///  Pointer Type
////////////////////////////////////////////////////////////////////////////////

template<class T>
class RPointerType :
    public gpg::RType,
    public gpg::RIndexed
{
public:
    using type = T;

    static gpg::RPointerType<type> *sPointerType = nulltpr;

    static RPointerType<T> GetType() {
        static RPointerType<T> PointerType{};
        if (sPointerType == nullptr) {
            sPointerType = gpg::LookupRType(typeid(type *));
        }
        return sPointerType;
    }

public:
    ~RPointerType() override = default;
    const char *GetName() override {
        static std::string sName{
            std::string{type::StaticGetClass()->GetName()} + "*"
        };
        return sName.c_str();
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
    gpg::RRef SubscriptIndex(void *, int) override {

    }
    size_t GetCount(void *) const override {

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




////////////////////////////////////////////////////////////////////////////////
///  Enum Type
////////////////////////////////////////////////////////////////////////////////

// 0x00D48CA0
class REnumType : public gpg::RType
{
public:
    struct ROptionValue
    {
        int mValue;
        const char *mName;
    };

public:
    const char *mPrefix;
    std::vector<gpg::REnumType::ROptionValue> mEnumNames;

    ~REnumType() override = default; // 0x00418120
    std::string GetLexical(const gpg::RRef &) const override; // 0x008E1C40
    bool SetLexical(const gpg::RRef &, const char *) const override; // 0x008D9670
    const gpg::REnumType *IsEnumType() const override; // 0x004180F0

    const std::vector<gpg::REnumType::ROptionValue> &GetEnumOptions() const; // 0x
    const char *StripPrefix(const char *) const; // 0x008D86F0
    bool GetEnumValue(const char *, int *) const; // 0x
    void AddEnum(char const *, int); // 0x008DF5F0
};

#define DECL_REFLECT_ENUM_TYPE(clazz) DECL_REFLECT_ENUM_TYPE_FOR(clazz##TypeInfo, clazz)
#define DECL_REFLECT_ENUM_TYPE_FOR(clazz, forClazz)\
class clazz : public gpg::REnumType\
{\
public:\
    using type = forClazz;\
\
    ~clazz() override = default;\
    const char *GetName() override {\
        return #forClazz;\
    }\
    void Init() override {\
        this->mSize = sizeof(type);\
        this->RType::Init();\
        this->AddEnums();\
        this->Finish();\
    }\
    void AddEnums();\
}

#define DECL_REFLECT_INCOM_ENUM_TYPE(clazz) DECL_REFLECT_INCOM_ENUM_TYPE(clazz##TypeInfo, clazz)
#define DECL_REFLECT_INCOM_ENUM_TYPE_FOR(clazz, forClazz)\
class clazz : public gpg::REnumType\
{\
public:\
    using type = forClazz;\
\
    ~clazz() override = default;\
    const char *GetName() override {\
        return #forClazz;\
    }\
    void Init() override {\
        this->mSize = sizeof(type);\
        this->RType::Init();\
        this->Finish();\
    }\
}




////////////////////////////////////////////////////////////////////////////////
///  Bool Type
////////////////////////////////////////////////////////////////////////////////

// 0x00D41874
class RBoolType : public gpg::RType
{
public:
    ~RBoolType() override = default; // 0x008DEFE0
    std::string GetLexical(const gpg::RRef &) const override; // 0x008DEF90
    bool SetLexical(const gpg::RRef &, const char *) const override; // 0x008DEEF0
};

#define DECL_REFLECT_BOOL_TYPE(clazz) DECL_REFLECT_BOOL_TYPE_FOR(clazz##TypeInfo, clazz)
#define DECL_REFLECT_BOOL_TYPE_FOR(clazz, forClazz)\
class clazz : public gpg::RBoolType\
{\
public:\
    using type = forClazz;\
\
    ~clazz() override = default;\
    const char *GetName() override {\
        return #forClazz;\
    }\
    void Init() override {\
        this->mSize = sizeof(type);\
        this->Finish();\
    }\
}




////////////////////////////////////////////////////////////////////////////////
///  Int Type
////////////////////////////////////////////////////////////////////////////////

template<class T>
class RIntType : public gpg::RType
{
public:
    ~RIntType() override = default;
    std::string GetLexical(const gpg::RRef &) const override {

    }
    bool SetLexical(const gpg::RRef &, const char *) const override {

    }
};

#define DECL_REFLECT_INT_TYPE(clazz) DECL_REFLECT_INT_TYPE_FOR(clazz##TypeInfo, clazz)
#define DECL_REFLECT_INT_TYPE_FOR(clazz, forClazz)\
class clazz : public gpg::RIntType\
{\
public:\
    using type = forClazz;\
\
    ~clazz() override = default;\
    const char *GetName() override {\
        return #forClazz;\
    }\
    void Init() override {\
        this->mSize = sizeof(type);\
        this->Finish();\
    }\
}




////////////////////////////////////////////////////////////////////////////////
///  Float Type
////////////////////////////////////////////////////////////////////////////////

// 0x00D418A8
class RFloatType : public gpg::RType
{
public:
    ~RFloatType() override = default; // 0x008DF150
    std::string GetLexical(const gpg::RRef &) const override; // 0x008DF080
    bool SetLexical(const gpg::RRef &, const char *) const override; // 0x008DF060
};




////////////////////////////////////////////////////////////////////////////////
///  String Type
////////////////////////////////////////////////////////////////////////////////

// 0x00D418D8
class RStringType : public gpg::RType
{
public:
    ~RStringType() override = default; // 0x008DF230
    std::string GetLexical(const gpg::RRef &) const override; // 0x008DF1C0
    bool SetLexical(const gpg::RRef &, const char *) const override; // 0x008DF200
};




////////////////////////////////////////////////////////////////////////////////
///  List Type
////////////////////////////////////////////////////////////////////////////////

template<class T>
class RListType : public gpg::RType
{
public:
    using type = T;

public:
    ~RListType() override = default;
    const char *GetName() override {
        static std::string sName{
            gpg::STR_Printf("list<%s>", type::StaticGetClass()->GetName())
        };
        return sName.c_str();
    }
    std::string GetLexical(const gpg::RRef &) const override {

    }
    void Init() override {

    }
};




////////////////////////////////////////////////////////////////////////////////
///  Map Type
////////////////////////////////////////////////////////////////////////////////

template<class K, class V>
class RMapType : public gpg::RType
{
public:
    using key_t = K;
    using value_t = V;

public:
    ~RMapType() override = default;
    const char *GetName() override {
        static std::string sName{gpg::STR_Printf("map<%s, %s>",
            key_t::StaticGetClass()->GetName(),
            value_t::StaticGetClass()->GetName()
        )};
        return sName.c_str();
    }
    std::string GetLexical(const gpg::RRef &) const override {

    }
    void Init() override {

    }
};




////////////////////////////////////////////////////////////////////////////////
///  Multimap Type
////////////////////////////////////////////////////////////////////////////////

template<class K, class V>
class RMultiMapType : public gpg::RType
{
public:
    using key_t = K;
    using value_t = V;

public:
    ~RMultiMapType() override = default;
    const char *GetName() override {
        static std::string sName{gpg::STR_Printf("multimap<%s, %s>",
            key_t::StaticGetClass()->GetName(),
            value_t::StaticGetClass()->GetName()
        )};
        return sName.c_str();
    }
    std::string GetLexical(const gpg::RRef &) const override {

    }
    void Init() override {

    }
};




////////////////////////////////////////////////////////////////////////////////
///  Vector Type
////////////////////////////////////////////////////////////////////////////////

template<class T>
class RVectorType : public gpg::RType, public gpg::RIndexed
{
public:
    using type = T;

public:
    ~RVectorType() override = default;
    const char *GetName() override {
        static std::string sName{
            gpg::STR_Printf("vector<%s>", type::StaticGetClass()->GetName())
        };
        return sName.c_str();
    }
    std::string GetLexical(const gpg::RRef &) const override {

    }
    gpg::RIndexed *IsIndexed() const override {
        return static_cast<gpg::RIndexed*>(this);
    }
    void Init() override {

    }
    gpg::RRef SubscriptIndex(void *, int) override {

    }
    size_t GetCount(void *) const override {

    }
    void SetCount(void *, int) const override {

    }
};




////////////////////////////////////////////////////////////////////////////////
///  Fast Vector Type
////////////////////////////////////////////////////////////////////////////////

template<class T>
class RFastVectorType : public gpg::RType, public gpg::RIndexed
{
public:
    using type = T;

public:
    ~RFastVectorType() override = default;
    const char *GetName() override {
        static std::string sName{
            gpg::STR_Printf("fastvector<%s>", type::StaticGetClass()->GetName())
        };
        return sName.c_str();
    }
    std::string GetLexical(const gpg::RRef &) const override {

    }
    gpg::RIndexed *IsIndexed() const override {
        return static_cast<gpg::RIndexed*>(this);
    }
    void Init() override {

    }
    void SubscriptIndex(void *, int) override {

    }
    size_t GetCount(void *) const override {

    }
    void SetCount(void *, int) const override {

    }
};


} // gpg


template<class T>
gpg::RRef func_RRef(void *ptr) {
    gpg::RType *type = T::StaticGetClass();
    if (ptr == nullptr || *typeid(ptr) == *typeid(T)) {
        this->mObj = ptr;
        this->mType = type;
    } else {
        std::type_info *rtType = typeid(ptr);
        thread_local std::pair<std::type_info *, gpg::RType *> tls[3];
        gpg::RType *derType;
        int k = 0;
        while (true) {
            if (tls[k]->first == rtType ||
                (tls[k]->first != nullptr && *tls[k]->first == *rtType)
            ) {
                derType = tls[k]->second;
                break;
            }
            if (k == 2) {
                derType = gpg::LookupRType(rtType);
                break;
            }
            k += 2;
        }
        if (k != 0) {
            do {
                tls[k].first = tls[k - 1].first;
                tls[k].second = tls[k - 1].second;
            } while (--k);
            tls[0].first = rtType;
            tls[0].second = derType;
        }
        int off = 0;
        bool isDer = derType->IsDerivedFrom(type, &off);
        GPG_ASSERT(isDer); // if (! isDer) { gpg::HandleAssertFailure("isDer", 458, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/reflection.h"); }
        this->mType = derType;
        this->mObj = &ptr[-off];
    }
}
