
namespace Moho {

// 0x00E207FC
class CUnitPodAssist : public Moho::CCommandTask
{
public:
    Moho::CCommandTask *mTask;
    Moho::WeakPtr<Moho::Unit> mUnit;
};

}
