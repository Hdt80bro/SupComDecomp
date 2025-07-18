
namespace Moho {

class CScriptObject : public Moho::TDatListItem<Moho::CScriptObject>
{
public:
    LuaPlus::LuaObject cobj;
    LuaPlus::LuaObject luaobj;
};


};