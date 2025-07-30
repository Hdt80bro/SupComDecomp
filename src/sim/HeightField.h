#include <vector>
#include "Wm3AxisAlignedBox3.h"
#include "core\MinMax.h"

struct struct_iGrid_data4
{
    Moho::SMinMax<unsigned short> *data;
    int width;
    int height;

    Moho::SMinMax<unsigned short> Get(int x, int y) {
        return this->data[x + y * this->width];
    }
};

struct struct_iGrid_data2
{
    __int16 *data;
    int width;
    int height;
};

struct struct_iGrid
{
    struct_iGrid_data4 data1;
    struct_iGrid_data2 data2;
};

namespace Moho {

struct CHeightField
{
    struct_iGrid_data2 grid;
    std::vector<struct_iGrid> grids;

    inline __int16 GetHeightAt(int x, int z) {
        return this->grid.data[x + z * this->grid.width];
    } // 0x00478490

    void InitField(int width, int height); // 0x004783D0
    Wm3::AxisAlignedBox3f GetTierBox(int x,int z, char a3); // 0x00475DF0
    Moho::SMinMax<unsigned short> GetTierBoundsUWord(int idx, int x, int y); // 0x00475BF0
};
    
};