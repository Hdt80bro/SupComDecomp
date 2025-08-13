#include "core/STIMap.h"
#include <memory>

namespace Moho {

// 0x00E4BC14
class IWldTerrainRes
{
public:
    std::auto_ptr<Moho::STIMap> mMap;

    virtual ~IWldTerrainRes() = default; // 0x0089E870
};

}