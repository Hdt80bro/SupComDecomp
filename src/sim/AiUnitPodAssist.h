
namespace Moho {

class CUnitPodAssist : public Moho::CCommandTask
{
public:
    Moho::CCommandTask *task;
    Moho::WeakPtr<Moho::Unit> unit;
};

}
