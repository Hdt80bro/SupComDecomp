#include "core/Stats.h"


namespace Moho {

class CountedObject
{
public:
    int mCount;

    virtual ~CountedObject() = default; // 0x004228E0

    CountedObject() : mCount{0} {} // 0x004228D0
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

};

template<class T>
struct CountedPtr
{
public: 
    T *mPtr;
};

}
