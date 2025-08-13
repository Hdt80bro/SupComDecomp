#include "boost/shared_ptr.hpp"
#include <string>

namespace Moho {

class RScmResource
{
    int v1;
    std::string mName;
    boost::shared_ptr<Moho::SScmFile> mFile;
    Moho::CAniSkel *mSkeleton;
};

}
