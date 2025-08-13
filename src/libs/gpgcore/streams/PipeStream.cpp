#include "PipeStream.h"

// 0x009565D0
gpg::PipeStream::PipeStream() :
    gpg::Stream{},
    mLock{},
    mSemaphore{},
    mBuf{}
{
    auto b = new struct_stream_buffer{};
    b->InsertBefore(&this->mBuf);
    this->mEnd = b->mData;
    this->mReadHead = b->mData;
    this->mStart = b->mData;
    this->mWriteStart = b->mData;
    this->mWriteHead = b->mData;
    this->mDataEnd = &b->mData[sizeof(b->mData)];
}

// 0x009569A0
gpg::PipeStream::~PipeStream() {
    while (this->mBuf.mNext != &this->mBuf) {
        auto cur = this->mBuf.mNext;
        if (cur) {
            delete(cur); // dtr moves next buffer to head
        }
    }
}

// 0x00483470
bool gpg::PipeStream::Empty() {
    return this->mReadHead == this->mEnd && this->VirtAtEnd();
}

// 0x009566C0
unsigned int gpg::PipeStream::GetLength() {
    boost::mutex::scoped_lock lock{this->mLock};
    unsigned int i = this->mWriteHead - this->mReadHead;
    if (this->mBuf.mNext != this->mBuf.mPrev) {
        i = &this->mEnd[i] - (char *)this->mBuf.mPrev - 8;
        for (auto cur = this->mBuf.mNext->mNext; cur != this->mBuf.mPrev; cur = cur->mNext) {
            i += 4096;
        }
    }
    return i;
}
