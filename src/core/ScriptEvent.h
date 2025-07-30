#include "core/ScriptObject.h"
#include "core/TaskEvent.h"

namespace Moho {

class CScriptEvent :
    public Moho::CTaskEvent,   // 0x00E09910
    public Moho::CScriptObject // 0x00E09918
{
    
};

}
