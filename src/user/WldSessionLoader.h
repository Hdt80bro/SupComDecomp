#include "user/WldSession.h"

namespace Moho {

class IWldSessionLoader
{
public:
    virtual void SetCreated() = 0;
    virtual struct_ScenarioInfo *GetScenarioInfo(const char* mapname, std::string *gamemods, bool set) = 0;
    virtual struct_ScenarioInfo *CreateScenarioInfo(const char* mapname, std::string *gamemods) = 0;
    virtual bool IsLoaded() = 0;
    virtual struct_GameData *LoadGameData(struct_GameData *) = 0;
    virtual void Func5() = 0;
    virtual void Func6() = 0;
};

class CWldSessionLoaderImpl : public Moho::IWldSessionLoader
{
public:
    bool mCreated;
    bool v1;
    bool v2;
    bool v3;
    int v4;
    struct_ScenarioInfo *v5;
    struct_ScenarioInfo *mGamedata;
    int v7;
    std::vector<unk_t> mOnTeardown;

    void SetCreated() override; // 0x00885890
    struct_ScenarioInfo *GetScenarioInfo(const char* mapname, std::string *gamemods, bool set) override; // 0x008858A0
    struct_ScenarioInfo *CreateScenarioInfo(const char* mapname, std::string *gamemods) override; // 0x00885920
    bool IsLoaded() override; // 0x00885970
    struct_GameData *LoadGameData(struct_GameData *) override; // 0x008859B0
    void Func5() override; // 0x00885AD0
    void Func6() override; // 0x008856E0
};

}

static Moho::CWldSessionLoaderImpl sWldSessionLoader;
