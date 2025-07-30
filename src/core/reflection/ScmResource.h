#include <string>
#include "boost/shared_ptr.hpp"

namespace Moho {

class RScmResource
{
    int v1;
    std::string name;
    boost::shared_ptr<Moho::SScmFile> file;
    Moho::CAniSkel *skeleton;
};

}
