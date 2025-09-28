#include "gpgcore/streams/PipeStream.h"
#include "zlib.h"

namespace gpg {
    
enum EFilterOperation
{
    FLOP_Inflate = 0,
    FLOP_Deflate = 1,
};

// 0x00D496F0
class ZLibOutputFilterStream : public gpg::Stream
{
public:
    gpg::PipeStream *mPipeStream;
    gpg::EFilterOperation mOperation;
    z_stream mZStream;
    char mBuff[1024];
    bool mEnded;
    bool mClosed;

    ~ZLibOutputFilterStream() override; // 0x00957340
    void VirtWrite(const char *data, size_t size) override; // 0x00957760
    void VirtFlush() override; // 0x00957810
    void VirtClose(gpg::Stream::Mode mode) override; // 0x009578B0

    ZLibOutputFilterStream(gpg::PipeStream *str, gpg::EFilterOperation op); // 0x00957360
    void DoWrite(const char *str, size_t len, int flush); // 0x00957500
};

}
