#include <string>
#include "xact3.h"
#include "TDatListItem.h"
#include "SndParams.h"


namespace Moho {

class HSound :
    public Moho::CScriptEvent,
    public Moho::TDatListItem<Moho::HSound>
{
    IXACTCue *ixactCue;
    Moho::CSndParams *params;
    bool v5;
};

}
