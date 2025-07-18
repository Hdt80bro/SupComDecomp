
namespace Moho {

class CAiReconDBImpl
{
    std::map<Moho::SReconKey, Moho::ReconBlip> blipMap;
    std::vector<Moho::ReconBlip> blips;
    std::vector<Moho::ReconBlip> v7;
    Moho::CArmyImpl *army;
    Moho::STIMap *mapdata;
    Moho::Sim *sim;
    Moho::CInfluenceMap *imap;
    boost::shared_ptr<Moho::CIntelGrid> m_visionGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_waterGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_radarGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_sonarGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_omniGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_rsiGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_sciGrid;
    boost::shared_ptr<Moho::CIntelGrid> m_vciGrid;
    Moho::EntityCategory v31;
    bool fogOfWar;
    int v42;

    virtual void ReconTick(int); // 0x005C0C40
    virtual void ReconRefresh(); // 0x005C14E0
    virtual int ReconCanDetect(gpg::Rect2i*, float elev, int type); // 0x005C18A0
    virtual int ReconCanDetect(Wm3::Vector3f* pos, Moho::EReconFlags oldFlags); // 0x005C1850
    virtual void ReconGetBlips(int);
    virtual void ReconGetBlips(int);
    virtual void ReconGetBlips(int);
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetVisionGrid(); // 0x005C1A10
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetWaterGrid(); // 0x005C1A40
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetRadarGrid(); // 0x005C1A70
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetSonarGrid(); // 0x005C1AA0
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetOmniGrid(); // 0x005C1AD0
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetRCIGrid(); // 0x005C1B00
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetSCIGrid(); // 0x005C1B30
    virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetVCIGrid(); // 0x005C1B60
    virtual void ReconSetFogOfWar(int);
    virtual void ReconGetFogOfWar(int);
    virtual void __unknown(int);
    virtual void ReconGetBlip(int);
    virtual void ReconGetJamingBlips(int);
    virtual void ReconFlushBlipsInRect(int);
};

}
