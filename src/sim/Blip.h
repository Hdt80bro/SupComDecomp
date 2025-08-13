#include "sim/Entity.h"


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
    bool mNeedsFlush;
    Moho::EReconFlags mReconFlags;
    int mSTIMesh;
    boost::shared_ptr<Moho::RScmResource> mMesh;
    boost::shared_ptr<Moho::RScmResource> mPriorPose;
    boost::shared_ptr<Moho::RScmResource> mPose;
    float mHealth;
    float mMaxHealth;
    float mFractionComplete;
    bool mMaybeDead;
};


// 0x00E1D824
// 0x00E1D8C8
class ReconBlip : public Moho::Entity
{
public:
    Moho::IUnit_chain *v1;
    Moho::IUnit_chain **v2;
    bool mIsMobile;
    Wm3::Vector3f mJamOffset;
    Moho::SSTIUnitConstantData mConstDat;
    Moho::SSTIUnitVariableData mVarDat;
    int v141;
    std::vector<Moho::SPerArmyRecon> mReconDat;
};

}
