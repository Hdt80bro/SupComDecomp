#include "ZLibStream.h"

// 0x00957340
gpg::ZLibOutputFilterStream::~ZLibOutputFilterStream() {
    this->Close(gpg::Stream::ModeBoth);
    if (this->operation == 0) {
        inflateEnd(&this->mZStream);
    } else if (this->operation == 1) {
        deflateEnd(&this->mZStream);
    }
}

// 0x00957760
void gpg::ZLibOutputFilterStream::VirtWrite(const char *str, size_t len) {
    if (this->closed) {
        throw std::runtime_error{std::string{"ZLibOutputFilterStream: stream closed."}};
    }
    if (this->writeStart != this->writeHead) {
        this->BufWrite(this->writeHead, this->writeStart - this->writeHead, Z_NO_FLUSH);
        this->writeStart = this->writeHead;
    }
    this->BufWrite(str, len, Z_NO_FLUSH);
}

// 0x00957810
void gpg::ZLibOutputFilterStream::VirtFlush() {
    if (this->closed) {
        throw std::runtime_error{std::string{"ZLibOutputFilterStream: stream closed."}};
    }
    this->BufWrite(this->writeHead, this->writeStart - this->writeHead, Z_SYNC_FLUSH);
    this->writeStart = this->writeHead;
}

// 0x009578B0
void gpg::ZLibOutputFilterStream::VirtClose(gpg::Stream::Mode mode) {
    if ((mode & gpg::Stream::ModeSend) != 0 && ! this->closed) {
        this->BufWrite(this->writeHead, this->writeStart - this->writeHead, Z_FINISH);
        if (this->operation == 0 && !this->v272a) {
            this->closed = true;
            throw std::runtime_error{std::string{"ZLibOutputFilterStream: stream closed before end."}};
        }
    }
}

// 0x00957360
gpg::ZLibOutputFilterStream::ZLibOutputFilterStream(gpg::PipeStream *strm, int operation) :
    gpg::Stream{}
{
    this->pipeStream = strm;
    this->operation = operation;
    this->v272a = false;
    this->closed = false;
    this->writeStart = this->buf;
    this->writeHead = this->buf;
    this->dataEnd = &this->buf[sizeof(this->buf)];
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
void gpg::ZLibOutputFilterStream::BufWrite(const char *str, size_t len, int flush) {
    char buf[1024];

    if (this->v272a) {
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
            this->mZStream.next_out = (unsigned __int8 *) buf;
            this->mZStream.avail_out = sizeof(buf);
            res = Z_STREAM_ERROR;
            if (this->operation == 0) {
                res = inflate(&this->mZStream, flush);
            } else if (this->operation == 1) {
                res = deflate(&this->mZStream, flush);
            }
            if (res != Z_OK) {
                break;
            }
            this->pipeStream->Write(buf, this->mZStream.next_out - (unsigned __int8 *) buf);
        }
        if (res != Z_BUF_ERROR) {
            if (res == Z_STREAM_END) {
                this->v272a = Z_STREAM_END;
                this->pipeStream->Write(buf, this->mZStream.next_out - (unsigned __int8 *) buf);
                if (this->mZStream.avail_in != 0) {
                    return;
                }
                throw std::runtime_error{std::string{"ZLibOutputFilterStream: Excess data after stream end."}};
            }
            throw std::runtime_error{std::string{"CDeflateOutputFilter::BufWrite(): call to deflate() failed."}};
        }
        if (this->mZStream.next_out == (unsigned __int8 *) buf) {
            break;
        }
        this->pipeStream->Write(buf, this->mZStream.next_out - (unsigned __int8 *) buf);
    }
    if (this->mZStream.avail_in != 0) {
        gpg::HandleAssertFailure("mZStream.avail_in == 0", 157, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\streams\\ZLibStream.cpp");
    }
}
