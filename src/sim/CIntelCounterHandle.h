
namespace Moho {

    enum EIntelCounter
    {
        INTELCOUNTER_RadarStealthField = 0x1,
        INTELCOUNTER_SonarStealthField = 0x2,
        INTELCOUNTER_CloakField = 0x8,
    };

    class CIntelCounterHandle : CIntelPosHandle
    {
        Moho::Sim *sim;
        Moho::CAiReconDBImpl *reconDB;
        Moho::EIntelCounter type;

        
        void __thiscall AddViz(Wm3::Vector3f *pos) override;
        void __thiscall SubViz(Wm3::Vector3f *pos) override;
    };

};