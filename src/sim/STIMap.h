
namespace Moho {

    struct TerrainTypes
    {
        LuaPlus::LuaObject v1;
        _BYTE gap14[5100];
        _DWORD v5;
        _DWORD v6;
        _DWORD v7;
        _DWORD v8;
    };

    struct STIMap
    {
        boost::shared_ptr<Moho::CHeightField> heightField;
        gpg::Rect2i playableRect;
        Moho::TerrainTypes terrainTypes;
        gpg::Array2D terrainType;
        bool blocking[256];
        bool waterEnabled;
        float waterElevation;
        float waterElevationDeep;
        float waterElevationAbyss;
        int v1;

        bool IsBlockingTerrain(unsigned int x, unsigned int z); // 0x00577F20
        void SetTerrainType(unsigned int x, unsigned int z, char type); // 0x00577EC01
        float GetTerrainTypeOffset(float x, float z); // 0x00758E90
    };
    
};