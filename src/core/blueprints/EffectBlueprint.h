#include "gpgcore/reflection/reflection.h"

namespace Moho {

class RTrailBlueprint;
class RBeamBlueprint;
class REmitterBlueprint;

class REffectBlueprint : public gpg::RObject
{
public:
    int gap;
    std::string mBlueprintId;
    bool mHighFidelity;
    bool mMedFidelity;
    bool mLowFidelity;

    virtual Moho::RTrailBlueprint *IsTrail() { return nullptr; } // 0x0050E620
    virtual Moho::RBeamBlueprint *IsBeam() { return nullptr; } // 0x0050E630
    virtual Moho::REmitterBlueprint *IsEmitter() { return nullptr; } // 0x0050E640
};

}