#include "RType.h"
#include "EnumValue.h"

namespace gpg {

struct REnumType : public gpg::RType
{
public:
    const char *prefix;
    std::vector<struct_EnumValue> enumnames;
};

};