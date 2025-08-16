#include "gpgcore/reflection/reflection.h"
#include <string>

namespace Moho {

// 0x00E0E0A8
class RBlueprint : public gpg::RObject
{
public:
    Moho::IRuleGameRules *mOwner;
    std::string mName;
    std::string mDesc;
    std::string mSource;
    unsigned int mBlueprintOrdinal;

    virtual void OnInitBlueprint() {} // 0x0050DBA0
};

}
