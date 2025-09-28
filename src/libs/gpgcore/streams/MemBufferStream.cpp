// known file

#include "MemBufferStream.h"
#include "gpgcore/General.h"


// 0x0008E5DC0
size_t gpg::MemBufferStream::VirtTell(gpg::Stream::Mode mode) {
    if (mode == gpg::Stream::ModeRead) {
        return this->BytesRead();
    } else if (mode == gpg::Stream::ModeWrite) {
        return this->BytesWritten();
    } else {
        throw std::invalid_argument{std::string{"invalid mode for MemBufferStream::Tell()"}};
    }
}

// 0x0008E6140
size_t gpg::MemBufferStream::VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, __int64 pos) {
    size_t seekpos = 0;

    __int64 end = this->BytesRead();
    __int64 readPos = end;
    if ((mode & gpg::Stream::ModeRead) != 0) {
        if (orig == gpg::Stream::OriginBegin) {
            readPos = pos;
        } else if (orig == gpg::Stream::OriginCurr) {
            readPos += pos;
        } else if (orig == gpg::Stream::OriginEnd) {
            readPos = end + pos;
        } else {
            throw std::invalid_argument{std::string{"invalid origin for MemBufferStream::Seek()"}};
        }
        if (readPos < 0) {
            throw std::invalid_argument{std::string{"attempt to seek to before the beginning of a stream."}};
        }
        seekpos = readPos;
    } else {
        readPos = pos;
    }

    __int64 writePos = this->BytesWritten();
    if ((mode & gpg::Stream::ModeWrite) != 0) {
        if (this->mInput.begin() == nullptr) {
            throw std::logic_error{std::string{"can't seek the output position on a read-only MemBufferStream."}};
        }
        if (orig == gpg::Stream::OriginBegin) {
            writePos = pos;
        } else if (orig == gpg::Stream::OriginCurr) {
            writePos += pos;
        } else if (orig == gpg::Stream::OriginEnd) {
            writePos = end + pos;
        } else {
            throw std::invalid_argument{std::string{"invalid origin for MemBufferStream::Seek()"}};
        }
        if (writePos < 0) {
            throw std::invalid_argument{std::string{"attempt to seek to before the beginning of a stream."}};
        }
        seekpos = writePos;
    } else {
        writePos = pos;
    }

    __int64 farthest = std::max(readPos, writePos);
    if (farthest > end) {
        if (this->mInput.begin() == nullptr) {
            throw std::runtime_error{std::string{"Can't seek past the end of a read-only MemBufferStream."}};
        }
        if (farthest > this->mInput.Size()) {
            this->Resize(farthest);
        }
        memset(&this->mInput[(size_t) end], 0, farthest - end);
    }

    this->mWritePtr = &this->mWriteStart[writePos];
    this->mReadPtr = &this->mReadStart[readPos];
    return seekpos;
}

// 0x008E5A50
size_t gpg::MemBufferStream::VirtRead(char * buf, size_t len) {
    if (this->mWritePtr != nullptr && this->mWritePtr > this->mReadEnd) {
        this->mReadEnd = this->mWritePtr;
    }
    if (this->LeftToRead() >= len) {
        memcpy(buf, this->mReadPtr, len);
        this->mReadPtr += len;
        return len;
    } else {
        memcpy(buf, this->mReadPtr, this->LeftToRead());
        this->mReadPtr = this->mReadEnd;
        return this->LeftToRead();
    }
}

// 0x008E5E70
void gpg::MemBufferStream::VirtUnGetByte(int) {
    throw std::runtime_error{std::string{"Attempt to UnGetByte() beyond the start of the buffer."}};
}

// 0x008E5AB0
bool gpg::MemBufferStream::VirtAtEnd() {
    if (this->mWritePtr != nullptr && this->mWritePtr > this->mReadEnd) {
        this->mReadEnd = this->mWritePtr;
    }
    return this->LeftToRead() == 0;
}

// 0x008E6470
void gpg::MemBufferStream::VirtWrite(const char *data, size_t bytes) {
    if (this->mInput.begin() == nullptr) {
        throw std::runtime_error{std::string{"Can't write to a read-only MemBufferStream."}};
    }
    if (&this->mWritePtr[bytes] > this->mWriteEnd) {
        this->Resize(&this->mWritePtr[bytes] - this->mWriteStart);
    }
    GPG_ASSERT(size_t(mWriteEnd - mWritePtr) >= bytes); // if (this->mWriteEnd - this->mWritePtr < bytes) { gpg::HandleAssertFailure("size_t(mWriteEnd - mWritePtr) >= bytes", 185, "c:\\work\\rts\\main\\code\\src\\libs\\gpgcore\\streams\\MemBufferStream.cpp"); }
    memcpy(this->mWritePtr, data, bytes);
    this->mWritePtr += bytes;
    if (this->mWritePtr > this->mReadEnd) {
        this->mReadEnd = this->mWritePtr;
    }
}

// 0x008E5AD0
void gpg::MemBufferStream::VirtFlush() {
    if (this->mWritePtr != nullptr && this->mWritePtr > this->mReadEnd) {
        this->mReadEnd = this->mWritePtr;
    }
}


// 0x0088B7E0
gpg::MemBuffer<const char> gpg::MemBufferStream::GetConstBuffer() {
    return gpg::MemBuffer<const char>{reinterpret_cast<const gpg::MemBuffer<const char> &>(this->mOutput)};
}

// 0x008E59F0
size_t gpg::MemBufferStream::GetLength() {
    if (this->mWritePtr != nullptr && this->mWritePtr > this->mReadEnd) {
        return this->BytesWritten();
    } else {
        return this->mReadEnd - this->mReadStart;
    }
}

// 0x008E5EE0
void gpg::MemBufferStream::Resize(__int64 size) {
    if (size > 0x7FFFFFFF) {
        throw std::runtime_error{std::string{"invalid position for MemBufferStream::Seek() -- buffer too large"}};
    }
    __int64 inSize = 2 * this->mInput.Size();
    while (inSize < size) {
        inSize *= 2;
    }
    if (size > 0x7FFFFFFF) {
        throw std::runtime_error{std::string{"invalid position for MemBufferStream::Seek() -- buffer too large"}};
    }
    gpg::MemBuffer<char> buff = gpg::AllocMemBuffer(inSize);
    memcpy(buff.begin(), this->mInput.begin(), this->GetLength());

    size_t savePtr = this->mWritePtr - this->mWriteStart;
    this->mInput = buff;
    this->mWriteStart = this->mInput.begin();
    this->mWritePtr = &this->mInput[savePtr];
    this->mWriteEnd = this->mInput.end();

    savePtr = this->mReadPtr - this->mReadStart;
    this->mOutput = buff;
    this->mReadStart = this->mOutput.begin();
    this->mReadPtr = &this->mOutput[savePtr];
    this->mReadEnd = &this->mOutput[(size_t) size];
}

// 0x008E5AE0
gpg::MemBufferStream::MemBufferStream(size_t size) :
    gpg::Stream{}
{
    this->mInput = gpg::AllocMemBuffer(size);
    this->mOutput = this->mInput;
    this->mReadStart = this->mInput.begin();
    this->mReadPtr = this->mInput.begin();
    this->mReadEnd = this->mInput.begin();
    this->mWriteStart = this->mInput.begin();
    this->mWritePtr = this->mInput.begin();
    this->mWriteEnd = &this->mInput.begin()[this->mInput.Size()];
}
