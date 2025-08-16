// known file

#include "gpgcore/General.h"

namespace gpg {

struct Array2D
{
    char *mData;
    int mSizeX;
    int mSizeY;

    char Get(int x, int y) {
        return this->mData[x + y * this->mSizeX];
    }
    char *At(int x, int y) {   
        GPG_ASSERT(x < mSizeX && y < mSizeY); // if (! (x < this->mSizeX && y < this->mSizeY)) { gpg::HandleAssertFailure("x < mSizeX && y < mSizeY", 135, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore/containers/Array2D.h"); }
        return &this->mData[x + y * this->mSizeX];
    }
};

struct BitArray2D
{
    int *mPtr;
    int mSize;
    int mWidth;
    int mHeight;
};


}
