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
        ModeRead = 1,
        ModeWrite = 2,
        ModeBoth = 3,
    };

    enum SeekOrigin
    {
        OriginBegin = 0,
        OriginCurr = 1,
        OriginEnd = 2,
    };

public:
    char *mReadStart;
    char *mReadPtr;
    char *mReadEnd;
    char *mWriteStart;
    char *mWritePtr;
    char *mWriteEnd;

    virtual ~Stream() = default; // 0x00956E20
    virtual size_t VirtTell(gpg::Stream::Mode mode); // 0x00956F50
    virtual size_t VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, __int64 pos); // 0x00956F90
    virtual size_t VirtRead(char *buff, size_t len); // 0x00956FB0
    virtual size_t VirtReadNonBlocking(char *buf, size_t len); // 0x00956DE0
    virtual void VirtUnGetByte(int); // 0x00956FD0
    virtual bool VirtAtEnd(); // 0x00956DF0
    virtual void VirtWrite(const char *data, size_t size); // 0x00956FF0
    virtual void VirtFlush(); // 0x00956E00
    virtual void VirtClose(gpg::Stream::Mode mode); // 0x00956E10

    Stream() = default; // 0x00956DB0
    bool CanRead() const { // inline
        return this->mReadEnd != this->mReadPtr;
    }
    int LeftToRead() const { // inline
        return this->mReadEnd - this->mReadPtr;
    }
    bool SomethingRead() const { // inline
        return this->mReadPtr != this->mReadStart;
    }
    int BytesRead() const { // inline
        return this->mReadPtr - this->mReadStart;
    }

    bool CanWrite() const { // inline
        return this->mWriteEnd != this->mWritePtr;
    }
    int LeftToWrite() const { // inline
        return this->mWriteEnd - this->mWritePtr;
    }
    bool SomethingWritten() const { // inline
        return this->mWritePtr != this->mWriteStart;
    }
    int BytesWritten() const { // inline
        return this->mWritePtr - this->mWriteStart;
    }
    void Write(const char *buf, size_t size); // 0x0043D130
    bool Close(gpg::Stream::Mode access); // 0x00955760
    size_t Read(char *buf, size_t size); // 0x0043D100
    size_t ReadNonBlocking(char *buf, size_t size) { // inline e.g. 0x0047BF13
        if (size > this->LeftToRead()) {
            size = this->VirtReadNonBlocking(buf, size);
        } else if (size) {
            memcpy(buf, this->mReadPtr, size);
            this->mReadPtr += size;
        }
        return size;
    }
    void Write(gpg::fastvector<char> &vec) { // inline
        this->Write(vec.mStart, vec.Size());
    }

};

}
