
namespace Moho {

    struct CIntel
    {
        Moho::CIntelPosHandle *v1;
        Moho::CIntelPosHandle *visionGrid;
        Moho::CIntelPosHandle *waterGrid;
        Moho::CIntelPosHandle *radarGrid;
        Moho::CIntelPosHandle *sonarGrid;
        Moho::CIntelPosHandle *omniGrid;
        Moho::CIntelCounterHandle *rciGrid;
        Moho::CIntelCounterHandle *sciGrid;
        Moho::CIntelCounterHandle *vciGrid;
        bool jamming;
        bool jammingEnabled;
        bool spoof;
        bool spoofEnabled;
        bool cloak;
        bool cloakEnabled;
        bool radarStealth;
        bool radarStealthEnabled;
        bool sonarStealth;
        bool sonarStealthEnabled;
    };

};