#include <string>
#include "gpgcore/reflection/RObject.h"

namespace Moho {

// 0x00E0E0A8
class RBlueprint : gpg::RObject
{
    Moho::RRuleGameRulesImpl *owner;
    std::string name;
    std::string desc;
    std::string source;
    unsigned int BlueprintOrdinal;
};

}
