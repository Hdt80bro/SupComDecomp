#include "core/MessageDispatcher.h"

namespace Moho {

// 0x00E6AF74
class IClientManager : public Moho::CMessageDispatcher
{
public:
    virtual ~IClientManager() = default; // 0x0053B680

};

// 0x00E16B64
class CClientManagerImpl : public Moho::IClientManager
{

};

}
