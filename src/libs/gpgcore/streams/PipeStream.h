#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"
#include "Stream.h"
#include "DatListItem.h"

struct struct_stream_buffer :
    gpg::DatListItem<struct_stream_buffer>
{
    char mData[4096];
};


namespace gpg {

// 0x00D495F0
class PipeStream : public gpg::Stream
{
public:
    boost::mutex mLock;
    int v11;
    boost::condition mSemaphore;
    gpg::DatListItem<struct_stream_buffer> mBuf;

    PipeStream(); // 0x009565D0
    ~PipeStream(); // 0x009569A0
    bool Empty(); // 0x00483470
    unsigned int GetLength(); // 0x009566C0
};

}
