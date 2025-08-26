#include "sim/Formation.h"

namespace Moho {

class CAiFormationInstance : public Moho::CFormationInstance
{
public:
    Moho::Sim *mSim;
    int mVal4;

    ~CAiFormationInstance() override; // 0x0059BD60
};

// 0x00E1B45C
class IAiFormationDB
{
public:
    virtual ~IAiFormationDB() = default; // 0x0059A3D0
};

// 0x00E1B52C
class CAiFormationDBImpl : public Moho::IAiFormationDB
{
public:
    Moho::Sim *mSim;
    gpg::fastvector_n<Moho::CAiFormationInstance, 10> mFormInstances;

    ~CAiFormationDBImpl() override = default; // 0x0059C340

};




}
