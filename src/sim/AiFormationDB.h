#include "gpgcore/containers/fastvector.h"

namespace Moho {

// 0x00E1B45C
class IAiFormationDB
{};

// 0x00E1B52C
class CAiFormationDBImpl : public Moho::IAiFormationDB
{
public:
    Moho::Sim *mSim;
    gpg::fastvector_n<Moho::CAiFormationInstance, 10> mFormInstances;
};
    
}
