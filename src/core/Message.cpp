// known file

#include "Message.h"
#include <stdexcept>
#include "gpgcore/streams/Stream.h"


Moho::CMessage::CMessage(int size, char type) {
    size += 3;
    char fill = 0;
    this->mBuf.Resize(size, &fill);       
    this->SetSize(size);
    this->SetType(type);
}

int Moho::CMessage::GetMessageSize() {
    int size = this->GetSize();
    if (size >= 3) {
        size -= 3;
    }
    return size;
}

unsigned int Moho::CMessage::Append(char *ptr, int size) {
    if (this->mBuf.Size() + size > 0x10000) {
        throw std::runtime_error{std::string{"Message too large"}};
    }
    this->mBuf.InsertAt(this->mBuf.mEnd, ptr, &ptr[size]);
    this->SetSize(this->mBuf.Size());
}

bool Moho::CMessage::ReadMessage(gpg::Stream *stream) {
    char fill = 0;
    this->mBuf.Resize(3, &fill);
    if (stream->Read(this->mBuf.mStart, 3) != 3) {
        return false;
    }
    int size = this->GetSize();
    if (size < 3) {
        return false;
    }
    if (size == 3) {
        return true;
    }
    this->mBuf.Resize(size - 3, &fill);
    return stream->Read(&this->mBuf[3], size - 3) == size - 3;
}

bool Moho::CMessage::Read(gpg::Stream *stream) {
    if (! this->HasReadLength()) {
        if (this->mBuf.Size() == 0) {
            char fill = 0;
            this->mBuf.Resize(3, &fill);
        }
        this->mPos += stream->ReadNonBlocking(&this->mBuf[this->mPos], 3 - this->mPos);
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
    this->mBuf.Resize(newSize, &fill);
    this->mPos += stream->ReadNonBlocking(&this->mBuf[this->mPos], newSize - this->mPos);
    return this->mPos == newSize;
}
