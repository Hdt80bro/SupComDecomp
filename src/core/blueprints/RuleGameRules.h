#include "core/Counted.h"
#include "core/DiskWatch.h"
#include "core/blueprints/Blueprint.h"
#include "core/blueprints/EffectBlueprint.h"
#include "core/blueprints/EntityCategory.h"
#include "core/blueprints/Footprint.h"
#include "core/blueprints/MeshBlueprint.h"
#include "core/blueprints/PropBlueprint.h"
#include "core/blueprints/ProjectileBlueprint.h"
#include "core/blueprints/UnitBlueprint.h"
#include "gpgcore/containers/fastvector.h"
#include "gpgcore/MD5.h"
#include "LuaPlus.h"
#include <vector>

namespace Moho {

// 0x00E1610C
class RRuleGameRules
{
public:
    Moho::CWldTerrainRes *v1;

    virtual ~RRuleGameRules() { Moho::InstanceCounter<Moho::RRuleGameRules>::Decrement(); } // 0x00528080
    virtual void ExportToLuaState(LuaPlus::LuaState *) = 0;
    virtual void UpdateLuaState(LuaPlus::LuaState *) = 0;
    virtual void CancelExport(LuaPlus::LuaState *) = 0;
    virtual int AssignNextOrdinal() = 0;
    virtual Moho::RBlueprint *GetBlueprintFromOrdinal(int) const = 0;
    virtual const Moho::SRuleFootprintsBlueprint *GetFootprints() const = 0;
    virtual const Moho::SNamedFootprint *FindFootprint(const Moho::SFootprint &, gpg::StrArg) const = 0;
    virtual const std::map<std::string, Moho::RUnitBlueprint *> &GetUnitBlueprints() = 0;
    virtual const std::map<std::string, Moho::RPropBlueprint *> &GetPropBlueprints() = 0;
    virtual const std::map<std::string, Moho::RProjectileBlueprint *> &GetProjectileBlueprints() = 0;
    virtual const std::map<std::string, Moho::RMeshBlueprint *> &GetMeshBlueprints() = 0;
    virtual Moho::REntityBlueprint *GetEntityBlueprint(const Moho::RResId &) = 0;
    virtual Moho::RUnitBlueprint *GetUnitBlueprint(const Moho::RResId &) = 0;
    virtual Moho::RPropBlueprint *GetPropBlueprint(const Moho::RResId &) = 0;
    virtual Moho::RMeshBlueprint *GetMeshBlueprint(const Moho::RResId &) = 0;
    virtual Moho::RProjectileBlueprint *GetProjectileBlueprint(const Moho::RResId &) = 0;
    virtual Moho::REmitterBlueprint *GetEmitterBlueprint(const Moho::RResId &) = 0;
    virtual Moho::RBeamBlueprint *GetBeamBlueprint(const Moho::RResId &) = 0;
    virtual Moho::RTrailBlueprint *GetTrailBlueprint(const Moho::RResId &) = 0;
    virtual Moho::REffectBlueprint *GetEffectBlueprint(const Moho::RResId &) = 0;
    virtual unsigned int GetUnitCount() const = 0;
    virtual const Moho::BVSet<const Moho::RBlueprint *, Moho::EntityCategoryHelper> &GetEntityCategory(const char *) const = 0;
    virtual Moho::BVSet<const Moho::RBlueprint *, Moho::EntityCategoryHelper> ParseEntityCategory(const char *) const = 0;
    virtual void UpdateChecksum(gpg::MD5Context &, _iobuf *) = 0;
};

class RRuleGameRulesImpl :
    public Moho::RRuleGameRules,    // 0x00E16174
    public Moho::CDiskWatchListener // 0x00E161DC
{
public:
    gpg::Mutex mLock;
    LuaPlus::LuaState *mLuaState;
    gpg::fastvector<unk_t> v17;
    Moho::SRuleFootprintsBlueprint mFootprints;
    std::map<std::string, Moho::RUnitBlueprint *> mUnitBlueprints;
    std::map<std::string, Moho::RProjectileBlueprint *> mProjectileBlueprints;
    std::map<std::string, Moho::RPropBlueprint *> mPropBlueprints;
    std::map<std::string, Moho::RMeshBlueprint *> mMeshBlueprints;
    std::map<std::string, Moho::REmitterBlueprint *> mEmitterBlueprints;
    std::map<std::string, Moho::RBeamBlueprint *> mBeamBlueprints;
    std::map<std::string, Moho::RTrailBlueprint *> mTrailBlueprints;
    std::map<std::string, Moho::RBlueprint *> mBlueprints;
    std::vector<Moho::EntityCategory> mCategories;

    ~RRuleGameRulesImpl() override; // 0x00529510
    void ExportToLuaState(LuaPlus::LuaState *) override; // 0x00529F70
    void UpdateLuaState(LuaPlus::LuaState *) override; // 0x0052A3D0
    void CancelExport(LuaPlus::LuaState *) override; // 0x0052AA20
    int AssignNextOrdinal() override; // 0x005282C0
    Moho::RBlueprint *GetBlueprintFromOrdinal(int) const override; // 0x0052B1A0
    const Moho::SRuleFootprintsBlueprint *GetFootprints() const override; // 0x005282E0
    const Moho::SNamedFootprint *FindFootprint(const Moho::SFootprint &, gpg::StrArg) const override; // 0x0052AAE0
    const std::map<std::string, Moho::RUnitBlueprint *> &GetUnitBlueprints() override; // 0x005282F0
    const std::map<std::string, Moho::RPropBlueprint *> &GetPropBlueprints() override; // 0x00528300
    const std::map<std::string, Moho::RProjectileBlueprint *> &GetProjectileBlueprints() override; // 0x00528320
    const std::map<std::string, Moho::RMeshBlueprint *> &GetMeshBlueprints() override; // 0x00528310
    Moho::REntityBlueprint *GetEntityBlueprint(const Moho::RResId &) override; // 0x0052AEB0
    Moho::RUnitBlueprint *GetUnitBlueprint(const Moho::RResId &) override; // 0x0052AB70
    Moho::RPropBlueprint *GetPropBlueprint(const Moho::RResId &) override; // 0x0052AD10
    Moho::RMeshBlueprint *GetMeshBlueprint(const Moho::RResId &) override; // 0x0052ADE0
    Moho::RProjectileBlueprint *GetProjectileBlueprint(const Moho::RResId &) override; // 0x0052AC40
    Moho::REmitterBlueprint *GetEmitterBlueprint(const Moho::RResId &) override; // 0x0052AEF0
    Moho::RBeamBlueprint *GetBeamBlueprint(const Moho::RResId &) override; // 0x0052AFC0
    Moho::RTrailBlueprint *GetTrailBlueprint(const Moho::RResId &) override; // 0x0052B090
    Moho::REffectBlueprint *GetEffectBlueprint(const Moho::RResId &) override; // 0x0052B160
    unsigned int GetUnitCount() const override; // 0x00528330
    const Moho::BVSet<const Moho::RBlueprint *, Moho::EntityCategoryHelper> &GetEntityCategory(const char *) const override; // 0x0052B1E0
    Moho::BVSet<const Moho::RBlueprint *, Moho::EntityCategoryHelper> ParseEntityCategory(const char *) const override; // 0x0052B280
    void UpdateChecksum(gpg::MD5Context &, _iobuf *) override; // 0x0052B2B0
};


}
