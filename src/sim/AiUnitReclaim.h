
namespace Moho {

class CUnitReclaimTask :
    public Moho::CCommandTask, // 0x00E20938
    struct_filler4,
    public Moho::Listener<Moho::ECommandEvent> // 0x00E20944
{
    int entityTarget;
    int v6;
    bool v7;
    Wm3::Vector3f pos;
    bool hasStarted;
    Moho::CEconRequest *consumptionData;
    float reclaimRate;
    float reclaimEnergy;
    float relcaimMass;
};



}
