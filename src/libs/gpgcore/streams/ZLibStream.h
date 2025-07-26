#include "gpgcore/streams/PipeStream.h"
#include "zlib.h"

namespace gpg {

class ZLibOutputFilterStream : public gpg::Stream
{
public:
    gpg::PipeStream *pipeStream;
    int operation;
    z_stream mZStream;
    char buf[1024];
    bool v272a;
    bool closed;

    ~ZLibOutputFilterStream() override; // 0x00957340
    void VirtWrite(const char *data, size_t size) override; // 0x00957760
    void VirtFlush() override; // 0x00957810
    void VirtClose(gpg::Stream::Mode mode) override; // 0x009578B0

    ZLibOutputFilterStream(gpg::PipeStream *str, int op); // 0x00957360
    void BufWrite(const char *str, size_t len, int flush); // 0x00957500
};

}
