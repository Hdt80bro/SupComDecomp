#include "core/Stats.h"
#include <type_traits>

namespace Moho {

class CountedObject
{
public:
    int mCount;

    virtual ~CountedObject() = default; // 0x004228E0

    CountedObject() : mCount{0} {} // 0x004228D0

    void Increment() {
        ++this->mCount;
    }
    void Decrement() {
        --this->mCount;
        if (this->mCount == 0) {
            delete this;
        }
    }
};

template<class T>
struct InstanceCounter
{
    using type = T;

    static Moho::StatItem *GetStatItem() {
        static Moho::StatItem *s_StatItem;

        if (s_StatItem == nullptr) {
            std::string name{"Instance Counts_"};
            for (auto k = typeid(type).name(); *k != '\0'; ++k) {
                if (*k != '_') {
                    name.append(1, *k);
                }
            }
            s_StatItem = func_GetEngineStats()->GetItem(name.c_str());
        }
        return s_StatItem;
    }

    static void Increment() {
        _InterlockedExchangeAdd(GetStatItem()->mCounter, 1);
    }

    static void Decrement() {
        _InterlockedExchangeAdd(GetStatItem()->mCounter, -1);
    }

};

template<class T>
struct CountedPtr
{
public:
    T *mPtr;

    CountedPtr(T *ptr) : mPtr{ptr} {
        this->mPtr->Increment();
    }
    ~CountedPtr() {
        this->mPtr->Decrement();
        this->mPtr = nullptr;
    }

    T &operator*() {
        return *this->mPtr;
    }
    T *operator->() {
        return &**this;
    }
    T *get() {
        return this->mPtr;
    }
};

template<class T>
bool operator==(Moho::CountedPtr<T> obj, std::nullptr_t) {
    return obj.get() == nullptr;
}

}
