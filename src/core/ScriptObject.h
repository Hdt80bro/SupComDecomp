#include "LuaPlus.h"
#include "core/TDatListItem.h"

namespace Moho {

// 0x00E0926C
class CScriptObject :
    public Moho::TDatListItem<Moho::CScriptObject>
{
public:
    LuaPlus::LuaObject cobj;
    LuaPlus::LuaObject luaobj;
};

}
