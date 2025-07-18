#include <Windows.h>

namespace gpg {

// likely an alias for `boost::mutex` 
struct Mutex
{
    LPCRITICAL_SECTION m_mutex;
    bool m_critical_section;
};

}
