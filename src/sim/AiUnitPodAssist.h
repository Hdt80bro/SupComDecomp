
namespace Moho {

// 0x00E207FC
class CUnitPodAssist : public Moho::CCommandTask
{
public:
    Moho::CCommandTask *task;
    Moho::WeakPtr<Moho::Unit> unit;
};

}
