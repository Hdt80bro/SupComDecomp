#include "core/Set.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

namespace Moho {

struct SSTICommandSource
{
    int mIndex;
    std::string mName;
    int mTimeouts;
};

struct SCommandSource
{
    std::vector<Moho::SSTICommandSource> mSrcs;
    int v4;
    int mOriginalSource;
};

class LaunchInfoNew;
class LaunchInfoLoad;

// 0x00E16E18
class LaunchInfoBase
{
public:
    Moho::RRuleGameRulesImpl *mGameRules;
    Moho::STIMap *mMap;
    std::string mGameMods;
    std::string mScenarioInfo;
    std::vector<Moho::BVIntSet> mArmyLaunchInfo;
    Moho::SCommandSource mCommandSources;
    std::string mLanguage;
    bool mCheatsEnabled;

    virtual ~LaunchInfoBase() = default; // 0x00542440
    virtual Moho::LaunchInfoNew *GetNew() { return nullptr; } // 0x00541FB0
    virtual Moho::LaunchInfoLoad* GetLoad() { return nullptr; } // 0x00541FC0
    virtual boost::shared_ptr<Moho::LaunchInfoBase> Create() = 0;
};

// 0x00E16DF0
class LaunchInfoNew : public Moho::LaunchInfoBase
{
public:
    std::vector<Moho::SPropInfo> *mProps;
    std::vector<std::string> mStrVec;
    int mInitSeed;

    ~LaunchInfoNew() override = default; // 0x005427F0
    Moho::LaunchInfoNew *GetNew() override { return this; } // 0x00541FD0
    boost::shared_ptr<Moho::LaunchInfoBase> Create() override; // 0x00542870
};

}
