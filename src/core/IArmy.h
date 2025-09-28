#include "core/IntelGrid.h"
#include "Wm3Vector2.h"
#include "boost/shared_ptr.hpp"
#include <string>


namespace Moho {

struct SSTIArmyConstantData
{
    int mIndex;
    std::string mArmyName;
    std::string mPlayerName;
    bool mIsCivilian;
    boost::shared_ptr<Moho::CIntelGrid> mVisionReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mWaterReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mRadarReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mSonarReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mOmniReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mRCIReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mSCIReconGrid;
    boost::shared_ptr<Moho::CIntelGrid> mVCIReconGrid;

};

struct SSTIArmyVariableData
{
    Moho::SEconTotals mData;
    int v47;
    int v48;
    Moho::BVIntSet mNeutrals;
    Moho::BVIntSet mAllies;
    Moho::BVIntSet mEnemies;
    bool mIsAlly;
    char v73b;
    char v73c;
    char v73d;
    int v74;
    Moho::BVIntSet mValidCommandSources;
    Moho::color_t mPlayerColor;
    Moho::color_t mArmyColor;
    std::string mArmyType;
    int mFaction;
    bool mUseWholeMap;
    char v93b;
    char v93c;
    char v93d;
    int v94;
    int v95;
    int v96;
    int v97;
    int v98;
    bool mShowScore;
    char v99b;
    char v99c;
    char v99d;
    int v100;
    int v101;
    int v102;
    int v103;
    int v104;
    int v105;
    int v106;
    int v107;
    int v108;
    int v109;
    int v110;
    bool mIsOutOfGame;
    char v11b;
    char v11c;
    char v11d;
    Wm3::Vector2f mArmyStart;
    int mNoRushTimer;
    float mNoRushRadius;
    float mNoRushOffsetX;
    float mNoRushOffsetY;
    float mHasHandicap;
    float mHandicap;
    int v120;
};


class IArmy
{
public:
    Moho::SSTIArmyConstantData mConstDat;
    Moho::SSTIArmyVariableData mVarDat;
};

}
