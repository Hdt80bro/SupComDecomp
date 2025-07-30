#include "sim/Entity.h"


struct struct_BlipData
{
    int v1;
    int creation_tick;
    bool autoMode;
    bool autoSurfaceMode;
    bool isBusy;
    float fuelRatio;
    float shieldRatio;
    int stunTicks;
    bool isPaused;
    bool isValidTarget;
    float dword1C;
    Moho::EFireState fireState;
    float workProgress;
    int tacticalSiloBuildCount;
    int nukeSiloBuildCount;
    int tacticalSiloStorageCount;
    int nukeSiloStorageCount;
    int tacticalSiloMaxStorageCount;
    int nukeSiloMaxStorageCount;
    int v2;
    std::string customName;
    float energyProduced;
    float massProduced;
    Moho::SEconValue resourcesSpent;
    float energyMaintainenceCost;
    float massMaintainanceCost;
    int focusUnit;
    int v3;
    int v4;
    boost::shared_ptr<Moho::RScmResource> priorSharedPose;
    boost::shared_ptr<Moho::RScmResource> sharedPose;
    Wm3::Vector3f vec3;
    _DWORD dwordA0;
    _DWORD dwordA4;
    _DWORD gapA8;
    _BYTE v0;
    _BYTE v1;
    _BYTE v2;
    _BYTE v3;
    _BYTE gapB0[24];
    _DWORD dwordC8;
    _DWORD dwordCC;
    _DWORD dwordD0;
    _DWORD dwordD4;
    _DWORD gapD8;
    _DWORD gapDC;
    _DWORD gapE0;
    _DWORD gapE4;
    _DWORD gapE8;
    float gapEC;
    _DWORD gapF0;
    _DWORD gapF4;
    std::vector<int> gapF8;
    _BYTE gap108[152];
    Moho::UnitAttributes attributes;
    unsigned int scriptbits;
    _BYTE gap214[4];
    Moho::EUnitStateMask unitStates;
    Moho::CUnitMotion *unitMotion;
};


namespace Moho {
  
enum EReconFlags
{
    RECON_None = 0x0,
    RECON_Radar = 0x1,
    RECON_Sonar = 0x2,
    RECON_RadarSonar = 0x3,
    RECON_Omni = 0x4,
    RECON_AnyPing = 0x7,
    RECON_LOSNow = 0x8,
    RECON_AnySense = 0xF,
    RECON_LOSEver = 0x10,
    RECON_Exposed = 0x1C,
    RECON_KnownFake = 0x20,
    RECON_MaybeDead = 0x40,
};

struct SPerArmyRecon
{
    bool needsFlush;
    Moho::EReconFlags mReconFlags;
    int sstimesh;
    boost::shared_ptr<Moho::RScmResource> mesh;
    boost::shared_ptr<Moho::RScmResource> priorPose;
    boost::shared_ptr<Moho::RScmResource> pose;
    float health;
    float maxHealth;
    float fractionComplete;
    bool MaybeDead;
};


// 0x00E1D824
// 0x00E1D8C8
class ReconBlip : public Moho::Entity
{
public:
    Moho::IUnit_chain *v1;
    Moho::IUnit_chain **v2;
    bool isMobile;
    Wm3::Vector3f jamOffset;
    Moho::SSTIUnitConstantData statItem;
    struct_BlipData recondata;
    int v141;
    std::vector<Moho::SPerArmyRecon> recondat;
};

}
