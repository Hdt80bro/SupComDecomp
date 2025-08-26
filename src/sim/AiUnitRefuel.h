#include "sim/CommandTask.h"
#include "sim/Unit.h"
#include "core/WeakPtr.h"

namespace Moho {

// 0x00E20A0C
class CUnitRefuel : public Moho::CCommandTask
{
public:
    Moho::WeakPtr<Moho::Unit> v1;
    bool v2;
    bool isCarrier;

    ~CUnitRefuel() override; // 0x00621250
    int Execute() override; // 0x00621490
};

}
