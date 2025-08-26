#include "sim/Sim.h"
#include "core/Counted.h"
#include "core/TDatListItem.h"
#include "gpgcore/containers/fastvector.h"
#include "Wm3Quaternion.h"
#include "LuaPlus.h"
#include <map>

namespace Moho {

enum EFormationType
{
    FORMTYPE_Surface = 0x0,
    FORMTYPE_Air = 0x1,
    FORMTYPE_Combo = 0x2,
};

enum EFormationdStatus/*sic*/
{
    FORMATIONSTATUS_FormationUpdated = 0x0,
    FORMATIONSTATUS_FormationAtGoal = 0x1,
};

class IFormationInstance :
    public Moho::CountedObject,
    public Moho::TDatListItem<Moho::IFormationInstance>
{
public:
    ~IFormationInstance() override = default; // 0x00565CA0
};

struct struct_FormationStruct1
{
    gpg::fastvector_n<unk_t, 38> mVec;
};

struct struct_FormationStruct2
{
    Moho::SCoordsVec2 mPos;
    float mSize;
    int unk;
};


class CFormationInstance : public Moho::IFormationInstance
{
public:
    LuaPlus::LuaState *mState;
    Moho::RRuleGameRules *mGamerules;
    Moho::EUnitCommandType mCommandType;
    int mVal1;
    gpg::fastvector_n<Moho::Unit*, 8> mUnits;
    struct_FormationStruct1 mVec[2];
    gpg::fastvector_n<struct_FormationStruct2, 64> mSlots;
    std::map<unk_t, unk_t> mMap1;
    std::map<unk_t, unk_t> mMap2;
    Wm3::Vector3f mPos1;
    Wm3::Quaternionf mOrientation;
    Wm3::Quaternionf mOrientationChng;
    std::string mScriptName;
    Moho::SCoordsVec2 mCoord;
    float mVal2;
    bool mPlanUpdate;
    int mMaxSize;
    int mVal3;

    ~CFormationInstance() override; // 0x00569430
};
struct SFormationScriptResult
{
    bool mSuccess;
    int mVal;
    gpg::fastvector_n<LuaPlus::LuaObject, 15> mObjs;
};

unsigned int FORMATION_GetNumScripts(LuaPlus::LuaState *, Moho::EFormationType); // 0x00575A30
const char *FORMATION_GetScriptName(LuaPlus::LuaState *, int, Moho::EFormationType); // 0x00575BD0
int FORMATION_GetScriptIndex(LuaPlus::LuaState *, gpg::StrArg, Moho::EFormationType); // 0x00575DB0
int FORMATION_PickTravelFormation(LuaPlus::LuaState *, Moho::EFormationType, float); // 0x00576010
int FORMATION_PickBestFormation(LuaPlus::LuaState *, Moho::EFormationType, float); // 0x00576350
Moho::SFormationScriptResult FORMATION_RunScript(LuaPlus::LuaState *, Moho::RRuleGameRules *, gpg::StrArg, const LuaPlus::LuaObject &); // 0x00576690

}
