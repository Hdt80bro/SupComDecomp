#include <string>
#include "boost/thread/mutex.hpp"

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
    int *dword4;
    int *dword8;
    int previousHeapInfo;
    int dword10;
    int *nextHeapInfo;
    int *dword18;
    int dword1C;
    int dword20;
    int counter;
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

    void Release(int val); // 0x004151E0
    void SetValue(std::string &disp);
};

}

std::string func_FmtByteSize(size_t size); // 0x008D1040
void func_GetHeapInfo(struct_HeapStats *stats, int); // 0x00958D60
