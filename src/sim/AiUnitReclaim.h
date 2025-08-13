
namespace Moho {

class CUnitReclaimTask :
    public Moho::CCommandTask, // 0x00E20938
    struct_filler4,
    public Moho::Listener<Moho::ECommandEvent> // 0x00E20944
{
    int mEntityTarget;
    int v6;
    bool v7;
    Wm3::Vector3f mPos;
    bool mHasStarted;
    Moho::CEconRequest *mConsumptionData;
    float mReclaimRate;
    float mReclaimEnergy;
    float mRelcaimMass;
};



}
