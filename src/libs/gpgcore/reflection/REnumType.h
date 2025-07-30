#include "gpgcore/reflection/RType.h"
#include "gpgcore/reflection/EnumValue.h"

namespace gpg {

// 0x00D48CA0
struct REnumType : public gpg::RType
{
public:
    const char *prefix;
    std::vector<struct_EnumValue> enumnames;
};

};