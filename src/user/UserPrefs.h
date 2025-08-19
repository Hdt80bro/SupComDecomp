#include "LuaPlus.h"
#include <list>
#include <string>

namespace Moho {

// 0x00E4E2EC
class IUserPrefs
{
public:
    virtual ~IUserPrefs() = 0;
    virtual std::string &GetStr1() = 0;
    virtual std::string &GetStr2() = 0;
    virtual bool GetBoolean(std::string *key, bool def) = 0;
    virtual int GetInteger(std::string *key, int def) = 0;
    virtual float GetNumber(std::string *key, float def) = 0;
    virtual int GetHex(std::string *key, int def) = 0;
    virtual std::string GetString(std::string *key, std::string *def) = 0;
    virtual std::list<std::string> *GetTable(std::string *key, std::list<std::string> *def) = 0;
    virtual void SetBoolean(std::string *loc, bool val) = 0;
    virtual void SetInteger(std::string *loc, int val) = 0;
    virtual void SetNumber(std::string *loc, int val) = 0;
    virtual void SetHex(std::string *loc, int val) = 0;
    virtual void SetString(std::string *loc, int val) = 0;
    virtual void SetTable(std::string *key, std::list<std::string> *val) = 0;
    virtual LuaPlus::LuaObject LookupCurrentOption(std::string *key) = 0;
    virtual LuaPlus::LuaObject LookupKey(std::string *key) = 0;
    virtual LuaPLus::LuaObject GetPrefsTable() = 0;
    virtual void SetObject(std::string *loc, LuaPlus::LuaObject *obj) = 0;
    virtual LuaPlus::LuaState &GetState() = 0;
};

class CUserPrefs : public Moho::IUserPrefs
{
public:
    std::string mStr0;
    std::string mStr1;
    LuaPlus::LuaState mState;
    LuaPlus::LuaObject mPrefs;

    ~CUserPrefs() override = default; // 0x008C8620
    std::string &GetStr1() override; // 0x008C7540
    std::string &GetStr2() override; // 0x008C7550
    bool GetBoolean(std::string *key, bool def) override; // 0x008C7560
    int GetInteger(std::string *key, int def) override; // 0x008C75F0
    float GetNumber(std::string *key, float def) override; // 0x008C7680
    int GetHex(std::string *key, int def) override; // 0x008C7710
    std::string GetString(std::string *key, std::string *def) override; // 0x008C7880
    std::list<std::string> *GetStringArr(std::string *key, std::list<std::string> *def) override; // 0x008C7980
    void SetBoolean(std::string *key, bool val) override; // 0x008C7D70
    void SetInteger(std::string *key, int val) override; // 0x008C7D90
    void SetNumber(std::string *key, int val) override; // 0x008C7DB0
    void SetHex(std::string *key, int val) override; // 0x008C7DD0
    void SetString(std::string *key, int val) override; // 0x008C7E40
    void SetStringArr(std::string *key, std::list<std::string> *val) override; // 0x008C7E60
    LuaPlus::LuaObject LookupCurrentOption(std::string *key) override; // 0x008C7EA0
    LuaPlus::LuaObject LookupKey(std::string *key) override; // 0x008C8040
    LuaPLus::LuaObject GetPrefsTable() override; // 0x008C8020
    void SetObject(std::string *key, LuaPlus::LuaObject *obj) override; // 0x008C7E80
    LuaPlus::LuaState &GetState() override; // 0x008C8610

    CUserPrefs(); // 0x008C7410

};


}
