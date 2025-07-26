#include <string>
#include "xact3.h"
#include "TDatListItem.h"
#include "SndParams.h"
#include "ScriptEvent.h"


namespace Moho {

class HSound :
    public Moho::CScriptEvent,
    public Moho::TDatListItem<Moho::HSound>
{
    IXACT3Cue *ixactCue;
    Moho::CSndParams *params;
    bool v5;
};

}
