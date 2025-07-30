
namespace Moho {

class RRuleGameRules
{
    Moho::CWldTerrainRes *v1;
};

class RRuleGameRulesImpl :
    public Moho::RRuleGameRules,
    public Moho::CDiskWatchListener
{
    gpg::Mutex lock;
    LuaPlus::LuaState *luaState;
    gpg::fastvector v17;
    Moho::SRuleFootprintsBlueprint footprints;
    gpg::fastvector<gpg::RUnitBlueprint> unitBlueprints;
    gpg::fastvector projectileBlueprints;
    gpg::fastvector propBlueprints;
    gpg::fastvector meshBlueprints;
    gpg::fastvector<unk_t> v37;
    gpg::fastvector<unk_t> v40;
    gpg::fastvector<unk_t> v43;
    gpg::fastvector_RBlueprint blueprints;
    std::vector<unk_t> categories;
};


}
