#include "LuaPlus.h"
#include "boost/thread/mutex.hpp"
#include <map>
#include <string>

struct struct_HeapStats
{
    int mRreserved;
    int mCommitted;
    int mTotal;
    int mInSmallBlocks;
    int mInUse;
};


namespace Moho {

enum EStatType
{
    STAT_TYPE_NONE = 0x0,
    STAT_TYPE_FLOAT = 0x1,
    STAT_TYPE_INT = 0x2,
    STAT_TYPE_STRING = 0x3,
};

// 0x00E00300
class StatItem
{
public:
    int *dword4;
    int *dword8;
    int previousHeapInfo;
    int dword10;
    int *nextHeapInfo;
    int *dword18;
    int dword1C;
    int dword20;
    int mCounter;
    std::string mValue;
    int val;
    std::string str2;
    int dword64;
    int dword68;
    int dword6C;
    int dword70;
    std::string mName;
    Moho::EStatType mType;
    volatile int mLockingVar;
    boost::mutex mLock;

    virtual ~StatItem(); // 0x00408840
    virtual void ToLua(int dead, __out LuaPlus::LuaObject &); // 0x00418BD0

    void Release(int val); // 0x004151E0
    void SetValue(std::string &disp);
};

template<class T>
class Stats
{
public:
    T *mItem;
    gpg::Mutex mLock;
    
    virtual void Destroy(const char *key);
};

class CArmyStatItem : public Moho::StatItem
{
public:
    std::map<unk_t, unk_t> mMap;
};


// 0x00E00314
template<>
class Stats<Moho::StatItem>
{
public:
    
};

// 0x00E2FA8C
template<>
class Stats<Moho::CArmyStatItem>;


class EngineStats : public Moho::Stats<Moho::StatItem>
{
public:
    std::string mLog;
    std::string mStr1;
    int mVal1;
    bool mVal2;


    EngineStats(); // 0x004088C0
};


void STAT_GetLuaTable(LuaPlus::LuaState *, Moho::StatItem *, LuaPlus::LuaObject &); // 0x0041B390
void STAT_Frame(); // 0x00415E60


}

static Moho::EngineStats *sEngineStats; // 0x010A67B8

Moho::EngineStats *func_GetEngineStats(); // 0x00408940
std::string func_FmtByteSize(size_t size); // 0x008D1040
void func_GetHeapInfo(struct_HeapStats *stats, int); // 0x00958D60
