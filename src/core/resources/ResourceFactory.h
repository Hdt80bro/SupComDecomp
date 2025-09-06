#include "gpgcore/reflection/reflection.h"
#include "boost/shared_ptr.hpp"

namespace Moho {

// 0x00E07614
class ResourceFactoryBase
{
public:
    gpg::RType *mType;
    gpg::RType *Type;

    virtual void Init() = 0;
    virtual boost::shared_ptr<void> Load(const char *filename) = 0;
    virtual boost::shared_ptr<void> Preload(const char *filename) = 0;
    virtual boost::shared_ptr<void> LoadFrom(const char *filename, boost::shared_ptr<void> from) = 0;
};

template<class T>
class ResourceFactory : public Moho::ResourceFactoryBase
{
public:
    using type = T;
    using preload_t = T;

public:
    void Init() override {
        this->mType = type::StaticGetClass();
        this->mPreloadType = preload_t::StaticGetClass();
    }
    boost::shared_ptr<type> Load(const char *filename) override {
        return this->LoadImpl(filename);
    }
    boost::shared_ptr<preload_t> Preload(const char *filename) override {
        return this->PreloadImpl(filename);
    }
    boost::shared_ptr<type> LoadFrom(boost::shared_ptr<preload_t> from) override {
        return this->LoadFromImpl(from);
    }
    virtual boost::shared_ptr<type> LoadImpl(const char *filename) = 0;
    virtual boost::shared_ptr<preload_t> PreloadImpl(const char *filename) {
        return this->LoadImpl(filename);
    }
    virtual boost::shared_ptr<type> LoadFromImpl(boost::shared_ptr<preload_t> from) {
        return from;
    }
};

template<class T, class P>
class ResourceFactoryPreload : public Moho::ResourceFactoryBase
{
public:
    using type = T;
    using preload_t = P;

    void Init() override {
        this->mType = type::StaticGetClass();
        this->mPreloadType = preload_t::StaticGetClass();
    }
    boost::shared_ptr<type> Load(const char *filename) override {
        return this->LoadImpl(filename);
    }
    boost::shared_ptr<preload_t> Preload(const char *filename) override {
        return this->PreloadImpl(filename);
    }
    boost::shared_ptr<type> LoadFrom(boost::shared_ptr<preload_t> from) override {
        return this->LoadFromImpl(from);
    }
    virtual boost::shared_ptr<type> LoadImpl(const char *filename) = 0;
    virtual boost::shared_ptr<preload_t> PreloadImpl(const char *filename) = 0;
    virtual boost::shared_ptr<type> LoadFromImpl(const char *filename, boost::shared_ptr<preload_t> from) = 0;
};

}
