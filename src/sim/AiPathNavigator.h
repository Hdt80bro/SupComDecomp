#include <vector>
#include "Wm3Vector3.h"
#include "gpgcore/containers/Rect.h"
#include "sim/AiPathFinder.h"

namespace Moho {

enum ESearchType
{
};

struct HPathCell
{
    int v0;
};

struct SNavGoal
{
    gpg::Rect2i mPos1;
    gpg::Rect2i mPos2;
    Moho::ELayer mLayer;
};


struct NavPath
{
    std::vector<Moho::HPathCell> mPath;
};

// 0x00E1C66C
class CAiPathNavigator
{
public:
    int v2;
    int v3;
    int state;
    Moho::CAiPathFinder *mPathFinder;
    Moho::NavPath v6;
    Moho::HPathCell mCurrentPos;
    Moho::HPathCell mTargetPos;
    Moho::HPathCell v12;
    Moho::SNavGoal v13;
    Moho::ELayer mLayer;
    Moho::Sim *mSim;
    int v24;
    int PSFC;
    int v26;
    int NFDS;
    float v28;
    int v29;
    int v30;
    int v31;
    Moho::WeakPtr<Moho::Unit> v32;
    Wm3::Vector3f v34;
    bool mIsInFormation;
    char v37b;
    char v37c;
    char v37d;
    char v38a;
    char v38b;
    char v38c;
    char v38d;
    Moho::ESearchType v39;
    int v40;
    int entityIdM7;
    int entityIdMD;
};    

}
