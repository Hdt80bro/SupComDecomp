#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"
#include "Stream.h"
#include "DatListItem.h"

struct struct_stream_buffer :
    gpg::DatListItem<struct_stream_buffer>
{
    char data[4096];
};


namespace gpg {

class PipeStream : public gpg::Stream
{
public:
    boost::mutex lock;
    int v11;
    boost::condition semaphore;
    gpg::DatListItem<struct_stream_buffer> buf;

    PipeStream(); // 0x009565D0
    ~PipeStream(); // 0x009569A0
    bool Empty(); // 0x00483470
    unsigned int GetLength(); // 0x009566C0
};

}
