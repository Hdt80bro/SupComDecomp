#include "core/IUnit.h"
#include "sim/Entity.h"
#include <map>
#include <string>

namespace Moho {

struct SInfoCache
{
    Moho::CFormationInstance *mFormationLayer;
    Moho::WeakPtr<Moho::Unit> v1;
    int v2;
    bool v3;
    float mTopSpeed;
    float v4;
    Wm3::Vector3f v5;
};


class Unit :
    public Moho::IUnit, // 0x00E2A574
    public Moho::Entity // 0x00E2A5EC 0x00E2A688
{
public:
    Moho::SSTIUnitConstantData mConstDat;
    Moho::SSTIUnitVariableData mVarDat;
    int v81;
    Moho::CUnitMotion *mUnitMotion;
    Moho::CUnitCommandQueue *mCommandQueue;
    Moho::WeakPtr<Moho::Unit> mCreator;
    Moho::WeakPtr<Moho::Unit> mTransportedBy;
    Moho::WeakPtr<Moho::Unit> mFerryUnit;
    Moho::TDatListItem<Moho::CScriptObject> mFocusEntity;
    Moho::WeakPtr<Moho::Entity> v92;
    Moho::WeakPtr<Moho::Unit> mGuardedUnit;
    Wm3::Vector3f v96;
    int v109;
    Moho::EntitySetTemplate<Moho::Unit> v100;
    Moho::CAiFormationInstance *mGuardFormation;
    bool v111;
    int mCreationTick;
    Moho::CEconStorage *mExtraStorage;
    int v114;
    Moho::CEconRequest *mConsumptionData;
    bool mConsumptionIsActive;
    bool mProductionActive;
    float mResourceConsumed;
    Moho::CAniActor *mAniActor;
    Moho::IAiAttacker *mAttacker;
    Moho::IAiCommandDispatch *mCommandDispatch;
    Moho::IAiNavigator *mNavigator;
    Moho::IAiSteering *mSteering;
    Moho::IAiBuilder *mBuilder;
    Moho::IAiSiloBuild *mSiloBuild;
    Moho::IAiTransport *mTransport;
    bool v126;
    float mTransportLoadFactor;
    std::map<std::string, float> mArmor;
    int v131;
    int v132;
    char mTerrainType;
    char mDebugAIStates;
    Moho::SInfoCache mInfo;
    gpg::Rect2i mOccupation;
    gpg::fastvector_n<Moho::WeakPtr<Moho::Entity>, 20> mBlipsInRange;
    int mBlipLastUpdateTick;
    int v193;
    gpg::fastvector_n<Moho::ReconBlip, 2> mReconblips;
    bool mIsnotPod;
    bool mIsEngineer;
    bool mIsNaval;
    bool mIsAir;
    bool mUsesGridBasedMotion;
    bool mIsMelee;
    bool mHasFocus;
    gpg::RRef v202;
    int v204;
    int v205;
    int v206;
    int v207;
    
};

}
