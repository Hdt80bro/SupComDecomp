#include "core/blueprints/Blueprint.h"
#include "gpgcore/containers/fastvector.h"

namespace Moho {

using EntId = unsigned int;
using EntIdUniverse = unsigned int;

class RRuleGameRules;


enum EEntityType
{
    ENTITYTYPE_Unit = 0x100,
    ENTITYTYPE_Prop = 0x200,
    ENTITYTYPE_Projectile = 0x400,
    ENTITYTYPE_Entity = 0x800,
};

template<class T, class U>
struct BVSet
{
    unsigned int mVal0;
    unsigned int mVal1;
    gpg::fastvector<T> mVec1;
    gpg::fastvector<U> mVec2;
};

struct EntityCategoryHelper
{
    Moho::RRuleGameRules *mRules;
};

using EntityCategory = Moho::BVSet<const Moho::RBlueprint *, Moho::EntityCategoryHelper>;
using EntityCategorySet = Moho::BVSet<const Moho::EntId, Moho::EntIdUniverse>;

}
