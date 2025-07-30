#include <stdexcept>
#include "gpgcore/containers/fastvector.h"

namespace gpg {

// 0x00D49658
class Stream
{
public:

    // 0x00D49684
    class UnsupportedOperation : public std::logic_error
    {
    public:
        UnsupportedOperation(); // 0x00956E40
    };

    enum Mode
    {
        ModeNone = 0,
        ModeReceive = 1,
        ModeSend = 2,
        ModeBoth = 3,
    };
    enum SeekOrigin
    {
        OriginBegin = 0,
        OriginCurr = 1,
        OriginEnd = 2,
    };
    
    char *start;
    char *readHead;
    char *end;
    char *writeHead;
    char *writeStart;
    char *dataEnd;

    inline int LeftInReadBuffer() {
        return this->end - this->readHead;
    }
    inline int LeftInWriteBuffer() {
        return this->dataEnd - this->writeStart;
    }

    virtual ~Stream() = default; // 0x00956E20
    virtual size_t VirtTell(gpg::Stream::Mode mode); // 0x00956F50
    virtual size_t VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, size_t pos); // 0x00956F90
    virtual size_t VirtRead(char * buf, size_t len); // 0x00956FB0
    virtual size_t VirtReadNonBlocking(char * buf, size_t len); // 0x00956DE0
    virtual void VirtUnGetByte(int); // 0x00956FD0
    virtual bool VirtAtEnd(); // 00956DF0
    virtual void VirtWrite(const char *data, size_t size); // 0x00956FF0
    virtual void VirtFlush(); // 0x00956E00
    virtual void VirtClose(gpg::Stream::Mode mode); // 0x00956E10

    void Write(const char *buf, size_t size); // 0x0043D130
    bool Close(gpg::Stream::Mode access); // 0x00955760
    size_t Read(char *buf, size_t size); // 0x0043D100
    size_t ReadNonBlocking(char *buf, size_t size) {
        if (size > this->LeftInReadBuffer()) {
            size = this->VirtReadNonBlocking(buf, size);
        } else if (size) {
            memcpy(buf, this->readHead, size);
            this->readHead += size;
        }
        return size;
    } // inline e.g. 0x0047BF13
    void Write(gpg::fastvector<char> &vec) {
        this->Write(vec.start, vec.Size());
    } // inline

    Stream() = default; // 0x00956DB0
};

}
