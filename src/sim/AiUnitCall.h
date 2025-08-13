
namespace Moho {

// 0x00E1FDB8
class CUnitCallAirStagingPlatform : public Moho::CCommandTask
{
public:
        Moho::IUnit_chain **platform;
        Moho::IUnit_chain **v1;
        int v2;
};

// 0x00E1FD6C
class CUnitCallTeleport : public Moho::CCommandTask
{
public:
    Moho::IUnit_chain v0;
    Moho::IUnit_chain **v1;
    int v2;
};

}
