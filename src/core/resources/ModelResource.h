#include "gpgcore/MemBuffer.h"
#include "Wm3AxisAlignedBox3.h"
#include "boost/shared_ptr.hpp"
#include <string>

namespace Moho {

struct SScmFile
{
    
};

struct RScmResource
{
    boost::shared_ptr<Moho::RScmResource> mPtr;
    std::string mName;
    boost::shared_ptr<Moho::SScmFile> mFile;
    Moho::CAniSkel *mSkeleton;
    Wm3::AxisAlignedBox3f mBox;
    float mSize;
};

}
