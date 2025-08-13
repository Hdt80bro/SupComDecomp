#include "core/IntelPosHandle.h"
#include "Wm3Vector3.h"

namespace Moho {

enum EIntelCounter
{
    INTELCOUNTER_RadarStealthField = 0x1,
    INTELCOUNTER_SonarStealthField = 0x2,
    INTELCOUNTER_CloakField = 0x8,
};

// 0x00E361D4
class CIntelCounterHandle : public Moho::CIntelPosHandle
{
public:
    Moho::Sim *mSim;
    Moho::IAiReconDB *mReconDB;
    Moho::EIntelCounter mType;

    
    void AddViz() override;
    void SubViz() override;
};

}
