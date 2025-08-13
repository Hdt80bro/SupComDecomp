#include <map>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "wm3Vector3.h"
#include "sim/Blip.h"
#include "sim/IntelGrid.h"
#include "sim/InfluenceMap.h"

namespace Moho {

struct SReconKey
{
    Moho::WeakPtr<Moho::Entity> ent;
    int id;
};

// 0x00E1D7C4
class IAiReconDB
{

};

// 0x00E1D8D4
class CAiReconDBImpl : public Moho::IAiReconDB
{
    std::map<Moho::SReconKey, Moho::ReconBlip> mBlipMap;
    std::vector<Moho::ReconBlip> mBlips;
    std::vector<Moho::ReconBlip> v7;
    Moho::IArmy *mArmy;
    Moho::STIMap *mMapData;
    Moho::Sim *mSim;
    Moho::CInfluenceMap *mIMap;
    boost::shared_ptr<Moho::CIntelGrid> mVisionGrid;
    boost::shared_ptr<Moho::CIntelGrid> mWaterGrid;
    boost::shared_ptr<Moho::CIntelGrid> mRadarGrid;
    boost::shared_ptr<Moho::CIntelGrid> mSonarGrid;
    boost::shared_ptr<Moho::CIntelGrid> mOmniGrid;
    boost::shared_ptr<Moho::CIntelGrid> mRCIGrid;
    boost::shared_ptr<Moho::CIntelGrid> mSCIGrid;
    boost::shared_ptr<Moho::CIntelGrid> mVCIGrid;
    Moho::EntityCategory v31;
    bool mFogOfWar;
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
