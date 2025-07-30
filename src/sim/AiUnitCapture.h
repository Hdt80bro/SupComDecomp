
namespace Moho {

class CUnitCaptureTask :
    public Moho::CCommandTask, // 0x00E1FF10
    struct_filler4,
    public Moho::Listener<Moho::ECommandEvent> // 0x00E1FF1C
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
