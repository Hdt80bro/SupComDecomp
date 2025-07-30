#include <string>
#include "core/reflection/EntityBlueprint.h"


namespace Moho {

struct RPropBlueprintDefense
{
    float MaxHealth;
    float Health;
};

struct RPropBlueprintDisplay
{
    std::string MeshBlueprint;
    float UniformScale;
};

struct RPropBlueprintEconomy
{
    float ReclaimMassMax;
    float ReclaimEnergyMax;
};


// 0x00E10F9C
class RPropBlueprint : public Moho::REntityBlueprint
{
public:
    // order is intentional
    Moho::RPropBlueprintDisplay Display;
    Moho::RPropBlueprintDefense Defense;
    Moho::RPropBlueprintEconomy Economy;
};

}
