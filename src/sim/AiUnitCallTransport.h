
namespace Moho {

// 0x00E1FD20
class CUnitCallLandTransport : public Moho::CCommandTask
{
public:
    Moho::IUnit_chain *v0;
    int v1;
    float mBeamupTime;
    Moho::VTransform v3;
    Moho::VTransform v10;
    int v17;
};

// 0x00E1FCD4
class CUnitCallTransport : public Moho::CCommandTask
{
public:
    int v19;
    int v20;
    bool v21;
    float mBeamupTime;
    Moho::VTransform v23;
    Moho::VTransform v30;
    int v37;
};

}
