
namespace Moho {

class CUnitCaptureTask :
    public Moho::CCommandTask,
    struct_filler4,
    public Moho::Listener<Moho::ECommandEvent>
{
public:
    Moho::WeakPtr<Moho::Entity> target;
    bool hasStarted;
    int capProgress;
    int capTime;
    Moho::CEconRequest *consumptionData;
    Moho::SEconValue rate;
    int v12;
};

}
