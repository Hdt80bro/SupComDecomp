#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"

namespace Moho {

struct SClientBottleneckInfo
{
    int mType;
    int v2;
    Moho::SSyncFilter_Subobj1 v3;
};


class IClientMgrUIInterface
{
public:
    virtual void NoteDisconnect(gpg::StrArg) {} // 0x0088B6D0
    virtual void Func2() {} // 0x0088B6E0
    virtual void ReceiveChat(gpg::StrArg, const gpg::MemBuffer<const char> &) {} // 0x0088B6F0
    virtual void NoteGameSpeedChange(int) {} // 0x0088B700
    virtual void ReportBottleneck(Moho::SClientBottleneckInfo *) {} // 0x0088B710
    virtual void ReportBottleneckCleared() {} // 0x0088B720
};

// 0x00E4A938
class CWldUiInterface : public Moho::IClientMgrUIInterface
{
public:

};

}
