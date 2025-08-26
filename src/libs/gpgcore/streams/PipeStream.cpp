#include "PipeStream.h"


// 0x009569A0
gpg::PipeStream::~PipeStream() {
    while (! this->mBuff.Empty()) {
        auto cur = this->mBuff.mNext->Get();
        if (cur) {
            delete(cur); // dtr moves next buffer to head
        }
    }
}

// 0x00956A50
size_t gpg::PipeStream::VirtRead(char *buff, size_t len) {
    this->DoRead(buff, len, true);
}

// 0x00956A70
size_t gpg::PipeStream::VirtReadNonBlocking(char *buff, size_t len) {
    this->DoRead(buff, len, false);
}

// 0x009568E0
bool gpg::PipeStream::VirtAtEnd() {
    boost::mutex::scoped_lock lock{this->mLock};
    return this->mClosed && this->mReadHead == this->mWriteStart;
}

// 0x00956AB0
void gpg::PipeStream::VirtWrite(const char *data, size_t size) {
    boost::mutex::scoped_lock lock{this->mLock};
    if (this->mClosed) {
        throw new std::runtime_error{"Can't write to a pipe stream after output has been closed."};
    }
    int left = this->LeftToWrite();
    if (left < size) {
        if (left != 0) {
            memcpy(mWriteHead, data, left);
            data += left;
            size -= left;
        }
        while (size >= 0x1000) {
            auto buff = new struct_stream_buffer{};
            buff->InsertBefore(&this->mBuff);
            memcpy(buff->begin(), data, 0x1000);
            data += 0x1000;
            size -= 0x1000;
        }
        auto buff = new struct_stream_buffer{};
        buff->InsertBefore(&this->mBuff);
        memcpy(buff->begin(), data, size);
        char *last = &buff->mData[size];
        this->mWriteHead = last;
        this->mWriteStart = last;
        this->mWriteEnd = buff->end();
    } else {
        memcpy(mWriteHead, data, size);
        this->mWriteHead += size;
        this->mWriteStart = this->mWriteHead;
    }
    this->mSemaphore.notify_all();
}

// 0x00956CC0
void gpg::PipeStream::VirtFlush() {
    boost::mutex::scoped_lock lock{this->mLock};
    if (this->mClosed) {
        throw new std::runtime_error{"Can't write to a pipe stream after output has been closed."};
    }
    if (this->LeftToFlush() != 0) {
        this->mWriteStart = this->mWriteHead;
        this->mSemaphore.notify_all();
    }
}
    
// 0x00956920
void gpg::PipeStream::VirtClose(gpg::Stream::Mode mode) {
    if ((mode & gpg::Stream::Mode::ModeSend) != 0) {
        boost::mutex::scoped_lock lock{this->mLock};
        if (! this->mClosed) {
            this->mClosed = true;
            this->mWriteStart = this->mWriteHead;
            this->mSemaphore.notify_all();
        }
    }
}



// 0x00483470
bool gpg::PipeStream::Empty() {
    return this->mReadHead == this->mReadEnd && this->VirtAtEnd();
}

// 0x009565D0
gpg::PipeStream::PipeStream() :
    gpg::Stream{},
    mLock{},
    mSemaphore{},
    mBuff{}
{
    auto b = new struct_stream_buffer{};
    b->InsertBefore(&this->mBuff);
    this->mReadEnd = b->begin();
    this->mReadHead = b->begin();
    this->mReadStart = b->begin();
    this->mWriteHead = b->begin();
    this->mWriteStart = b->begin();
    this->mWriteEnd = b->end();
}

// 0x009566C0
size_t gpg::PipeStream::GetLength() {
    boost::mutex::scoped_lock lock{this->mLock};
    size_t i = this->mWriteStart - this->mReadHead;
    if (! this->mBuff.Empty()) {
        i = &this->mReadEnd[i] - (char *) this->mBuff.mPrev - 8; // ?
        for (auto cur = this->mBuff.mNext->mNext; cur != this->mBuff.mPrev; cur = cur->mNext) {
            i += 4096;
        }
    }
    return i;
}

// 0x00956740
size_t gpg::PipeStream::DoRead(char *buff, size_t len, bool doWait) {
    boost::mutex::scoped_lock lock{this->mLock};
    int amt = 0;
    for (int i = this->mReadEnd - this->mReadHead; i < len; i = this->LeftToRead()) {
        if (i != 0) {
            memcpy(buff, this->mReadHead, i);
            buff += i;
            amt += i;
            len -= i;
            this->mReadHead += i;
        }
        struct_stream_buffer *cur = this->mBuff.mNext->Get();
        if (this->mBuff.HasNext()) {
            if (this->mReadEnd != cur->end()) {
                this->mReadEnd = cur->end();
            } else {
                if (cur != nullptr) {
                    delete(cur);
                }
                cur = this->mBuff.mNext->Get();
                this->mReadHead = cur->begin();
                this->mReadStart = cur->begin();
                if (this->mBuff.HasNext()) {
                    this->mReadEnd = cur->end();
                } else {
                    this->mReadEnd = this->mWriteStart;
                }
            }
        } else if (this->mReadEnd != this->mWriteStart) {
            this->mReadEnd = this->mWriteStart;
        } else if (doWait && ! this->mClosed) {
            this->mSemaphore.wait(this->mLock);
        } else {
            return amt;
        }
    }
    if (len != 0) {
        memcpy(buff, this->mReadHead, len);
        this->mReadHead += len;
        amt += len;
    }
    return amt;
}
