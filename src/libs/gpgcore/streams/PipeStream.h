#include "gpgcore/streams/Stream.h"
#include "gpgcore/DList.h"
#include "boost/thread/condition.hpp"
#include "boost/thread/mutex.hpp"


namespace gpg {
    
struct PipeStreamBuffer :
    gpg::DListItem<gpg::PipeStreamBuffer, void>
{
    char mData[4096];

    char *begin() {
        return this->mData;
    }
    char *end() {
        return &this->mData[sizeof(this->mData)];
    }
};

// 0x00D495F0
class PipeStream : public gpg::Stream
{
public:
    boost::mutex mLock;
    bool mClosed;
    boost::condition mSemaphore;
    gpg::DList<gpg::PipeStreamBuffer, void> mBuff;

    ~PipeStream() override; // 0x00956A90
    size_t VirtRead(char *buff, size_t len) override; // 0x00956A50
    size_t VirtReadNonBlocking(char *buff, size_t len) override; // 0x00956A70
    bool VirtAtEnd() override; // 0x009568E0
    void VirtWrite(const char *data, size_t size) override; // 0x00956AB0
    void VirtFlush() override; // 0x00956CC0
    void VirtClose(gpg::Stream::Mode mode) override; // 0x00956920

    bool Empty(); // 0x00483470 ??
    PipeStream(); // 0x009565D0
    size_t GetLength(); // 0x009566C0
    size_t DoRead(char *buff, size_t len, bool doWait); // 0x00956740
};

}
