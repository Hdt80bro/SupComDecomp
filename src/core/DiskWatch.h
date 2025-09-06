#include "core/TDatListItem.h"
#include <string>

namespace Moho {

struct SDiskWatchEvent
{

};

struct CDiskWatch : Moho::TDatListItem<Moho::CDiskWatch>
{
    int mVal;
    struct_SharedLock mLock;

    bool EnablePrivileges() const; // 0x00462F80
    bool EnablePrivilege(const std::string &, bool) const; // 0x00463180
};

// 0x00E03474
class CDiskWatchListener : public Moho::TDatListItem<Moho::CDiskWatchListener>
{
public:
    Moho::CDiskWatch *mWatch;
    std::vector<Moho::SDiskWatchEvent> mEvents;
    std::vector<std::string> mPatterns;

    virtual void OnEvent(const Moho::SDiskWatchEvent &); // 0x00461DC0
    virtual bool FilterEvent(const Moho::SDiskWatchEvent &); // 0x00461D00
    virtual void OnDiskWatchEvent(const Moho::SDiskWatchEvent &); // 0x00461DF0
};

}

