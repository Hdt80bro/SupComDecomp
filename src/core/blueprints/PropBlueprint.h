#include "core/blueprints/EntityBlueprint.h"
#include <string>


namespace Moho {

struct RPropBlueprintDefense
{
    float mMaxHealth;
    float mHealth;
};

struct RPropBlueprintDisplay
{
    std::string mMeshBlueprint;
    float mUniformScale;
};

struct RPropBlueprintEconomy
{
    float mReclaimMassMax;
    float mReclaimEnergyMax;
};


// 0x00E10F9C
class RPropBlueprint : public Moho::REntityBlueprint
{
public:
    Moho::RPropBlueprintDisplay mDisplay;
    Moho::RPropBlueprintDefense mDefense;
    Moho::RPropBlueprintEconomy mEconomy;
};

}
