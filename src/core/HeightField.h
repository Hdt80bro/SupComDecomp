#include "core\MinMax.h"
#include "Wm3AxisAlignedBox3.h"
#include <vector>

struct struct_iGrid_data4
{
    Moho::SMinMax<unsigned short> *mData;
    int mWidth;
    int mHeight;

    Moho::SMinMax<unsigned short> Get(int x, int y) {
        return this->mData[x + y * this->mWidth];
    }
};

struct struct_iGrid_data2
{
    __int16 *mData;
    int mWidth;
    int mHeight;
};

struct struct_iGrid
{
    struct_iGrid_data4 mData1;
    struct_iGrid_data2 mData2;
};

namespace Moho {

struct CHeightField
{
    struct_iGrid_data2 mGrid;
    std::vector<struct_iGrid> mGrids;

    inline __int16 GetHeightAt(int x, int z) {
        return this->mGrid.mData[x + z * this->mGrid.mWidth];
    } // 0x00478490

    void InitField(int width, int height); // 0x004783D0
    Wm3::AxisAlignedBox3f GetTierBox(int x,int z, char a3); // 0x00475DF0
    Moho::SMinMax<unsigned short> GetTierBoundsUWord(int idx, int x, int y); // 0x00475BF0
};
    
}
