#include <string>
#include "xact3.h"
#include "core/ScriptEvent.h"
#include "core/SndParams.h"
#include "core/TDatListItem.h"


namespace Moho {

// 0x00E0B964
// 0x00E0B96C
class HSound :
    public Moho::CScriptEvent,
    public Moho::TDatListItem<Moho::HSound>
{
    IXACT3Cue *ixactCue;
    Moho::CSndParams *params;
    bool v5;
};

}
