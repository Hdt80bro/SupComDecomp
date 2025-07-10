
namespace Moho {

    class CUnitCaptureTask : Moho::CCommandTask, struct_filler4, Moho::Listener<Moho::ECommandEvent>
    {
        Moho::WeakPtr<Moho::Entity> target;
        bool hasStarted;
        int capProgress;
        int capTime;
        Moho::CEconRequest *consumptionData;
        Moho::SEconValue rate;
        int v12;
    };

};