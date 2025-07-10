
namespace Moho {

    struct CInfluenceMap
    {
        Moho::CArmyImpl *army;
        int total;
        int width;
        int height;
        int gridSize;
        std::map<uint, int> v5;
        std::vector<Moho::InfluenceGrid> mapEntries;

        float __XMM0 __userpurge GetThreatRect(
            Moho::CInfluenceMap *this,
            int x,
            int z __EAX,
            int radius,
            char onMap,
            Moho::EThreatType threatType,
            int army) asm("0x00715FF0");
    };

};