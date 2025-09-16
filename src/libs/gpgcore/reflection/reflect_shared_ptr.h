// known file

#include "gpgcore/reflection/reflection.h"

namespace gpg {

template<class T>
class RSharedPointerType : public gpg::RType, public gpg::RIndexed
{
public:
    type = T;
    ptr_type = boost::shared_ptr<void>;

public:
    ~RSharedPointerType() override = default;
    const char *GetName() override {
        static std::string sName{
            gpg::STR_Printf("boost::shared_ptr<%s>", func_GetType<type>()->GetName())
        };
        return sName.c_str();
    }
    std::string GetLexical(const gpg::RRef &ref) const override {
        auto obj = (ptr_type *) ref.mObj;
        if (obj()) {
            std::string name = func_GetType<type>()->GetLexical(gpg::RRef<T>{obj()});
            std::string ret{"["};
            ret += name;
            return ret + "]";
        } else {
            return std::string{"NULL"};
        }
    }
    gpg::RIndexed *IsIndexed() const override {
        return static_cast<gpg::RIndexed*>(this);
    }
    gpg::RIndexed *IsPointer() const override {
        return static_cast<gpg::RIndexed*>(this);
    }
    void Init() override {
        this->mSize = sizeof(ptr_type); 
    }
    gpg::RRef SubscriptIndex(void *ptr, int index) override {
        GPG_ASSERT(index == 0); // if (index != 0) { gpg::HandleAssertFailure("index == 0", 65, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/reflection/reflect_shared_ptr.h"); }
        auto obj = (ptr_type *) ptr;
        return gpg::RRef<T>{obj()};
    }
    size_t GetCount(void *ptr) const override {
        auto obj = (ptr_type *) ptr;
        return obj() != nullptr;
    }
};

}
