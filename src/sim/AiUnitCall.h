
namespace Moho {

// 0x00E1FDB8
class CUnitCallAirStagingPlatform : public Moho::CCommandTask
{
public:
        Moho::IUnit_chain **platform;
        Moho::IUnit_chain **v1;
        int v2;
};

// 0x00E1FD20
class CUnitCallLandTransport : public Moho::CCommandTask
{
public:
    Moho::IUnit_chain *v0;
    int v1;
    float beamupTime;
    Moho::VTransform v3;
    Moho::VTransform v10;
    int v17;
};

// 0x00E1FD6C
class CUnitCallTeleport : public Moho::CCommandTask
{
public:
    Moho::IUnit_chain v0;
    Moho::IUnit_chain **v1;
    int v2;
};

// 0x00E1FCD4
class CUnitCallTransport : public Moho::CCommandTask
{
public:
    int v19;
    int v20;
    bool v21;
    float beamupTime;
    Moho::VTransform v23;
    Moho::VTransform v30;
    int v37;
};

}
