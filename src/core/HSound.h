
namespace Moho {

    class HSound : CScriptEvent, TDatListItem<HSound>
    {
        IXACTCue *ixactCue;
        CSndParams *params;
        bool v5;
    };

};