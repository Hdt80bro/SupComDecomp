#include <string>
#include <map>
#include "boost/shared_ptr.hpp"
#include "gpgcore/reflection/RType.h"
#include "gpgcore/containers/fastvector.h"
#include "gpgcore/String.h"


static std::map<std::string, gpg::RType *> prefetchTypeMap; // 0x010C7940
static std::map<std::string, gpg::RType *> *pPrefetchTypeMap; // 0x010A6390

namespace Moho {

class CResourceWatcher
{
    int gap;
    gpg::fastvector_n<int, 2> vec; // unknown type

    virtual void OnResourceChanged(gpg::StrArg) = 0;
};

struct PrefetchData
{
    DWORD v0;
    DWORD v1;
    DWORD v2;
    DWORD v3;
    DWORD v4;
};

struct PrefetchHandleBase
{
    boost::shared_ptr<Moho::PrefetchData> dat;
};

void RES_RegisterPrefetchType(const char *, gpg::RType *); // 0x004A5060
std::string RES_CompletePath(gpg::StrArg, gpg::StrArg); // 0x004A9350
void RES_Exit(); // 0x004ABEB0
boost::shared_ptr<void> RES_GetResource(gpg::StrArg, Moho::CResourceWatcher *, const gpg::RType *); // 0x004ABEE0
Moho::PrefetchHandleBase RES_PrefetchResource(gpg::StrArg, const gpg::RType *); // 0x004ABF30

}

std::map<std::string, gpg::RType *> *func_GetPrefetchTypeMap(); // 0x004A4FB0
