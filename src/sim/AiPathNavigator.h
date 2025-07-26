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
    gpg::Rect2i pos1;
    gpg::Rect2i pos2;
    Moho::ELayer layer;
};


struct NavPath
{
    std::vector<Moho::HPathCell> path;
};

class CAiPathNavigator
{
public:
    DWORD v2;
    DWORD v3;
    DWORD state;
    Moho::CAiPathFinder *pathFinder;
    Moho::NavPath v6;
    Moho::HPathCell mCurrentPos;
    Moho::HPathCell mTargetPos;
    Moho::HPathCell v12;
    Moho::SNavGoal v13;
    Moho::ELayer layer;
    Moho::Sim *sim;
    int v24;
    DWORD PSFC;
    DWORD v26;
    DWORD NFDS;
    float v28;
    int v29;
    int v30;
    DWORD v31;
    Moho::WeakPtr<Moho::Unit> v32;
    Wm3::Vector3f v34;
    bool isInFormation;
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
