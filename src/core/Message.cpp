// known file

#include "Message.h"
#include "gpgcore/streams/Stream.h"
#include <stdexcept>


// 0x00483490
Moho::CMessage::CMessage(int size, Moho::EMessageOp type) {
    size += 3;
    char fill = 0;
    this->mBuff.Resize(size, &fill);       
    this->SetSize(size);
    this->SetType(type);
}

// 0x0047BD40
bool Moho::CMessage::ReadMessage(gpg::Stream *stream) {
    char fill = 0;
    this->mBuff.Resize(3, &fill);
    if (stream->Read(this->mBuff.mStart, 3) != 3) {
        return false;
    }
    int size = this->GetSize();
    if (size < 3) {
        return false;
    }
    if (size == 3) {
        return true;
    }
    this->mBuff.Resize(size - 3, &fill);
    return stream->Read(&this->mBuff[3], size - 3) == size - 3;
}

// 0x0047BDE0
unsigned int Moho::CMessage::Append(char *ptr, int size) {
    if (this->mBuff.Size() + size > 0x10000) {
        throw std::runtime_error{std::string{"Message too large"}};
    }
    this->mBuff.InsertAt(this->mBuff.mEnd, ptr, &ptr[size]);
    this->SetSize(this->mBuff.Size());
}

// 0x0047BE90
int Moho::CMessage::GetMessageSize() {
    int size = this->GetSize();
    if (size >= 3) {
        size -= 3;
    }
    return size;
}

// 0x0047BEE0
bool Moho::CMessage::Read(gpg::Stream *stream) {
    if (! this->HasReadLength()) {
        if (this->mBuff.Size() == 0) {
            char fill = 0;
            this->mBuff.Resize(3, &fill);
        }
        this->mPos += stream->ReadNonBlocking(&this->mBuff[this->mPos], 3 - this->mPos);
        if (! this->HasReadLength()) {
            return false;
        }
    }
    int newSize = this->GetSize();
    if (newSize < 3) {
        return false;
    }
    if (newSize == this->mPos) {
        return true;
    }
    char fill = 0;
    this->mBuff.Resize(newSize, &fill);
    this->mPos += stream->ReadNonBlocking(&this->mBuff[this->mPos], newSize - this->mPos);
    return this->mPos == newSize;
}


// 0x0047C0F0
size_t Moho::CMessageStream::VirtRead(char *buff, size_t len) {
    if (len > this->LeftToRead()) {
        len = this->LeftToRead();
    }
    memcpy(buff, this->mReadPtr, len);
    this->mReadPtr += len;
    return len;
}

// 0x0047C120
bool Moho::CMessageStream::VirtAtEnd() {
    return ! this->CanRead();
}

// 0x0047C130
void Moho::CMessageStream::VirtWrite(const char *buff, size_t len) {
    if (this->mWriteStart == nullptr) {
        throw std::logic_error{std::string{"Can't write to a read-only message."}};
    }
    size_t avail = std::min(this->LeftToWrite(), len);
    size_t amt = len - avail;
    if (len != avail) {
        this->mMsg->Append(&buff[avail], amt);
        char *dat = this->mMsg->Data();
        char *datEnd = &dat[this->mMsg->GetMessageSize()];
        this->mReadStart = dat;
        this->mReadPtr = &dat[this->BytesRead()];
        this->mReadEnd = datEnd;
        this->mWriteStart = dat;
        this->mWritePtr = &dat[this->BytesWritten() + amt];
        this->mWriteEnd = datEnd;
    }
}


// 0x0047BFE0
Moho::CMessageStream::CMessageStream(Moho::CMessage *msg) :
    gpg::Stream{},
    mMsg{msg}
{
    char *dat = msg->Data();
    char *datEnd = &dat[msg->GetMessageSize()];
    this->mReadStart = dat;
    this->mReadPtr = dat;
    this->mReadEnd = datEnd;
    this->mWriteStart = dat;
    this->mWritePtr = dat;
    this->mWriteEnd = datEnd;
}

// 0x0047C060
Moho::CMessageStream::CMessageStream(Moho::CMessage *msg, Moho::CMessageStream::ReadOnlyTag) :
    gpg::Stream{},
    mMsg{msg}
{
    char *dat = msg->Data();
    char *datEnd = &dat[msg->GetMessageSize()];
    this->mReadStart = dat;
    this->mReadPtr = dat;
    this->mReadEnd = datEnd;
}
