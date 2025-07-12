#include "Stream.h"
#include "gpgcore/Mutex.h"
#include "boost/thread/condition.hpp"

struct struct_stream_buffer
{
    void *v0;
    void *v1;
    int data[1024];
};


namespace gpg {

class PipeStream : public gpg::Stream
{
    gpg::Mutex lock;
    DWORD v11;
    boost::condition semaphore;
    struct_stream_buffer *buffer;
    struct_stream_buffer **pBuffer;
};

};