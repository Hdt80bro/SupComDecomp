#include "user/ClientManager.h"
#include "boost/shared_ptr.hpp"
#include <string>

struct struct_ScenarioInfo : Moho::TDatListItem<struct_ScenarioInfo>
{
    std::string mMapName;
    std::string mGameMods;
    bool mVal1;
    bool mVal2;
    int mVal3;
    Moho::CWldMap *mWldMap;
    LuaPlus::LuaState *mState;
    Moho::IRuleGameRules *mGameRules;
};

struct struct_GameData
{
    LuaPlus::LuaState *mState;
    Moho::IRuleGameRules *mGameRules;
    Moho::CWldMap *mWldMap;
};


namespace Moho {

struct SWldSessionInfo
{
    std::string mMapname;
    boost::shared_ptr<Moho::SFileStarCloser> mLaunchInfo;
    bool mIsBeingRecorded;
    bool mIsReplay;
    bool mIsMultiplayer;
    Moho::IClientManager *mClientManager;
    unsigned int mSourceId;
};

class CWldSession
{

};

}
