#include "gpgcore/General.h"

namespace gpg {

struct Array2D
{
    char *data;
    int mSizeX;
    int mSizeY;

    char Get(int x, int y) {
        return this->data[x + y * this->mSizeX];
    }
    char *At(int x, int y) {   
        if (x >= this->mSizeX || y >= this->mSizeY) {
            gpg::HandleAssertFailure("x < mSizeX && y < mSizeY", 135, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/containers/Array2D.h");
        }
        return &this->data[x + y * this->mSizeX];
    }
};

struct BitArray2D
{
    int *ptr;
    int size;
    int width;
    int height;
};


}
