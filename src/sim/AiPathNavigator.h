
namespace Moho {

    struct NavPath
    {
        std::vector<Moho::HPathCell> path;
    };

    class CAiPathNavigator
    {
        _DWORD v2;
        _DWORD v3;
        _DWORD state;
        Moho::CAiPathFinder *pathFinder;
        Moho::NavPath v6;
        Moho::HPathCell mCurrentPos;
        Moho::HPathCell mTargetPos;
        Moho::HPathCell v12;
        Moho::SNavGoal v13;
        Moho::ELayer layer;
        Moho::Sim *sim;
        int v24;
        _DWORD PSFC;
        _DWORD v26;
        _DWORD NFDS;
        float v28;
        int v29;
        int v30;
        _DWORD v31;
        Moho::WeakPtr_Unit v32;
        Wm3::Vector3f v34;
        _BYTE isInFormation;
        _BYTE v37b;
        _BYTE v37c;
        _BYTE v37d;
        _BYTE v38a;
        _BYTE v38b;
        _BYTE v38c;
        _BYTE v38d;
        Moho::ESearchType v39;
        int v40;
        int entityIdM7;
        int entityIdMD;
    };    

};