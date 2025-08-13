#include "boost/shared_ptr.hpp"
#include "gpgcore/streams/WriteArchive.h"

// 0x00D479C4
class TextWriteArchive : public gpg::WriteArchive {
public:
    int v7;
    boost::shared_ptr<std::ostream> mPtr;
    std::ostream *v9;

    ~TextWriteArchive() noexcept override = default; // 0x
};

namespace gpg {

gpg::WriteArchive *CreateTextWriteArchive(const boost::shared_ptr<std::ostream> &); // 0x00939280

}
