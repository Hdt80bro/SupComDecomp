#include "gpgcore/containers/fastvector.h"
#include "LuaPlus.h"

namespace Moho {

class IRuleGameRules
{
    Moho::CWldTerrainRes *v1;
};

class RRuleGameRulesImpl :
    public Moho::RRuleGameRules,
    public Moho::CDiskWatchListener
{
public:
    gpg::Mutex mLock;
    LuaPlus::LuaState *mLuaState;
    gpg::fastvector v17;
    Moho::SRuleFootprintsBlueprint mFootprints;
    gpg::fastvector<gpg::RUnitBlueprint> mUnitBlueprints;
    gpg::fastvector mProjectileBlueprints;
    gpg::fastvector mPropBlueprints;
    gpg::fastvector mMeshBlueprints;
    gpg::fastvector<unk_t> v37;
    gpg::fastvector<unk_t> v40;
    gpg::fastvector<unk_t> v43;
    gpg::fastvector<Moho::RBlueprint> blueprints;
    std::vector<unk_t> categories;
};


}
