// known file

#include "ZLibStream.h"
#include "gpgcore/General.h"

// 0x00957340
gpg::ZLibOutputFilterStream::~ZLibOutputFilterStream() {
    this->Close(gpg::Stream::ModeBoth);
    if (this->mOperation == 0) {
        inflateEnd(&this->mZStream);
    } else if (this->mOperation == 1) {
        deflateEnd(&this->mZStream);
    }
}

// 0x00957760
void gpg::ZLibOutputFilterStream::VirtWrite(const char *str, size_t len) {
    if (this->mClosed) {
        throw std::runtime_error{std::string{"ZLibOutputFilterStream: stream closed."}};
    }
    if (this->LeftToFlush()) {
        this->DoWrite(this->mWriteStart, this->LeftToFlush(), Z_NO_FLUSH);
        this->mWriteHead = this->mWriteStart;
    }
    this->DoWrite(str, len, Z_NO_FLUSH);
}

// 0x00957810
void gpg::ZLibOutputFilterStream::VirtFlush() {
    if (this->mClosed) {
        throw std::runtime_error{std::string{"ZLibOutputFilterStream: stream closed."}};
    }
    this->DoWrite(this->mWriteStart, this->LeftToFlush(), Z_SYNC_FLUSH);
    this->mWriteStart = this->mWriteHead;
}

// 0x009578B0
void gpg::ZLibOutputFilterStream::VirtClose(gpg::Stream::Mode mode) {
    if ((mode & gpg::Stream::ModeSend) != 0 && ! this->mClosed) {
        this->DoWrite(this->mWriteStart, this->LeftToFlush(), Z_FINISH);
        if (this->mOperation == 0 && !this->mEnded) {
            this->mClosed = true;
            throw std::runtime_error{std::string{"ZLibOutputFilterStream: stream closed before end."}};
        }
    }
}

// 0x00957360
gpg::ZLibOutputFilterStream::ZLibOutputFilterStream(gpg::PipeStream *strm, int operation) :
    gpg::Stream{},
    mPipeStream{strm},
    mOperation{operation},
    mEnded{false},
    mClosed{false}
{
    this->mWriteHead = this->mBuff;
    this->mWriteStart = this->mBuff;
    this->mWriteEnd = &this->mBuff[sizeof(this->mBuff)];
    memset(&this->mZStream, 0, sizeof(this->mZStream));
    if (operation == 0) {
        if (inflateInit2_(&this->mZStream, -14, "1.2.3", 56) != 0) {
            throw std::runtime_error{std::string{"ZLibOutputFilterStream: inflateInit2() failed."}};
        }
    } else if (operation == 1) {
        if (deflateInit2_(&this->mZStream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -14, 8, 0, "1.2.3", 56) != 0) {
            throw std::runtime_error{std::string{"ZLibOutputFilterStream: deflateInit2() failed."}};
        }
    } else {
        throw std::logic_error{std::string{"invalid operation"}};
    }
}

// 0x00957500
void gpg::ZLibOutputFilterStream::DoWrite(const char *str, size_t len, int flush) {
    char buff[1024];

    if (this->mEnded) {
        if (len == 0) {
            return;
        }
        throw std::runtime_error{std::string{"ZLibOutputFilterStream: excess data after stream end."}};
    }
    this->mZStream.next_in = (unsigned __int8 *) str;
    this->mZStream.avail_in = len;
    while (true) {
        int res;
        while (true) {
            this->mZStream.next_out = (unsigned __int8 *) buff;
            this->mZStream.avail_out = sizeof(buff);
            res = Z_STREAM_ERROR;
            if (this->mOperation == 0) {
                res = inflate(&this->mZStream, flush);
            } else if (this->mOperation == 1) {
                res = deflate(&this->mZStream, flush);
            }
            if (res != Z_OK) {
                break;
            }
            this->mPipeStream->Write(buff, this->mZStream.next_out - (unsigned __int8 *) buff);
        }
        if (res == Z_BUF_ERROR) {
            throw std::runtime_error{std::string{"CDeflateOutputFilter::BufWrite(): call to deflate() failed."}};
        }
        if (res == Z_STREAM_END) {
            this->mEnded = Z_STREAM_END;
            this->mPipeStream->Write(buff, this->mZStream.next_out - (unsigned __int8 *) buff);
            if (this->mZStream.avail_in != 0) {
                return;
            }
            throw std::runtime_error{std::string{"ZLibOutputFilterStream: Excess data after stream end."}};
        }
        if (this->mZStream.next_out == (unsigned __int8 *) buff) {
            break;
        }
        this->mPipeStream->Write(buff, this->mZStream.next_out - (unsigned __int8 *) buff);
    }
    GPG_ASSERT(mZStream.avail_in == 0); // if (this->mZStream.avail_in != 0) { gpg::HandleAssertFailure("mZStream.avail_in == 0", 157, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\streams\\ZLibStream.cpp"); }
}
