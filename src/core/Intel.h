#include "core/IntelCounterHandle.h"
#include "core/IntelPosHandle.h"

namespace Moho {

struct CIntel
{
    Moho::CIntelPosHandle *v1; // unknown; used as start for array of all grids
    Moho::CIntelPosHandle *mVisionGrid;
    Moho::CIntelPosHandle *mWaterGrid;
    Moho::CIntelPosHandle *mRadarGrid;
    Moho::CIntelPosHandle *mSonarGrid;
    Moho::CIntelPosHandle *mOmniGrid;
    Moho::CIntelCounterHandle *mRCIGrid; // radar counter-intel
    Moho::CIntelCounterHandle *mSCIGrid; // sonar counter-intel
    Moho::CIntelCounterHandle *mVCIGrid; // vision counter-intel
    bool mJamming;
    bool mJammingEnabled;
    bool mSpoof;
    bool mSpoofEnabled;
    bool mCloak;
    bool mCloakEnabled;
    bool mRadarStealth;
    bool mRadarStealthEnabled;
    bool mSonarStealth;
    bool mSonarStealthEnabled;
};

}
