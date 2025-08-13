#include "gpgcore/reflection/RObject.h"
#include <string>

namespace Moho {

// 0x00E0E0A8
class RBlueprint : public gpg::RObject
{
public:
    Moho::RRuleGameRulesImpl *mOwner;
    std::string mName;
    std::string mDesc;
    std::string mSource;
    unsigned int mBlueprintOrdinal;
};

}
