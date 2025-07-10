
namespace Moho {

    struct GridPos
    {
        int x;
        int z;

        GridPos(Wm3::Vector3f *wldPos, int gridSize); // 0x00506E20
    };

    struct CIntelGrid
    {
        Moho::STIMap *mapdata;
        char *grid;
        int width;
        int height;
        std::vector<Moho::CIntelUpdate> updateList;
        int gridSize;

        void Raster(Wm3::Vector3f *worldPos, int gridRadius, bool doAdd); // 0x00507540
        

        inline void SubtractCircle(Wm3::Vector3f *pos, unsigned int radius) {
            this->Raster(pos, radius / this->gridSize, false);
        } // 0x00507690
        
        inline void AddCircle(Wm3::Vector3f *pos, unsigned int radius) {
            this->Raster(pos, radius / this->gridSize, true);
        } // 0x00507670
    };

};