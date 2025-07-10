
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

        virtual void ReconTick(int) asm("0x005C0C40");
        virtual void ReconRefresh() asm("0x005C14E0");
        virtual int ReconCanDetect(gpg::Rect2i*, float elev, int type) asm("0x005C18A0");
        virtual int ReconCanDetect(Wm3::Vector3f* pos, Moho::EReconFlags oldFlags) asm("0x005C1850");
        virtual void ReconGetBlips(int) asm("0x");
        virtual void ReconGetBlips(int) asm("0x");
        virtual void ReconGetBlips(int) asm("0x");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetVisionGrid() asm("0x005C1A10");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetWaterGrid() asm("0x005C1A40");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetRadarGrid() asm("0x005C1A70");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetSonarGrid() asm("0x005C1AA0");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetOmniGrid() asm("0x005C1AD0");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetRCIGrid() asm("0x005C1B00");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetSCIGrid() asm("0x005C1B30");
        virtual boost::shared_ptr<Moho::CIntelGrid> ReconGetVCIGrid() asm("0x005C1B60");
        virtual void ReconSetFogOfWar(int) asm("0x");
        virtual void ReconGetFogOfWar(int) asm("0x");
        virtual void __unknown(int) asm("0x");
        virtual void ReconGetBlip(int) asm("0x");
        virtual void ReconGetJamingBlips(int) asm("0x");
        virtual void ReconFlushBlipsInRect(int) asm("0x");
    };

};