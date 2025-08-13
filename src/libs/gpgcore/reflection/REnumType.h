#include "gpgcore/reflection/RType.h"
#include "gpgcore/reflection/EnumValue.h"

namespace gpg {

// 0x00D48CA0
class REnumType : public gpg::RType
{
public:
    const char *mPrefix;
    std::vector<struct_EnumValue> mEnumNames;
};

};