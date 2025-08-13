
namespace Moho {

class CUnitCaptureTask :
    public Moho::CCommandTask, // 0x00E1FF10
    struct_filler4,
    public Moho::Listener<Moho::ECommandEvent> // 0x00E1FF1C
{
public:
    Moho::WeakPtr<Moho::Entity> mTarget;
    bool mHasStarted;
    int mCapProgress;
    int mCapTime;
    Moho::CEconRequest *mConsumptionData;
    Moho::SEconValue mRate;
    int v12;
};

}
