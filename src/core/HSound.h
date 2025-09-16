#include "core/ScriptEvent.h"
#include "core/SndParams.h"
#include "core/TDatList.h"
#include "xact3.h"
#include <string>


namespace Moho {

// 0x00E0B964
// 0x00E0B96C
class HSound :
    public Moho::CScriptEvent,
    public Moho::TDatListItem<Moho::HSound, void>
{
public:
    IXACT3Cue *mXactCue;
    Moho::CSndParams *mParams;
    bool v5;
};

}
