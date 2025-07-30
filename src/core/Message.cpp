#include "Message.h"
#include <stdexcept>
#include "gpgcore/streams/Stream.h"


Moho::CMessage::CMessage(int size, char type) {
    size += 3;
    char fill = 0;
    this->buf.Resize(size, &fill);       
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
    if (this->buf.Size() + size > 0x10000) {
        throw std::runtime_error{std::string{"Message too large"}};
    }
    this->buf.InsertAt(this->buf.end, ptr, &ptr[size]);
    this->SetSize(this->buf.Size());
}

bool Moho::CMessage::ReadMessage(gpg::Stream *stream) {
    char fill = 0;
    this->buf.Resize(3, &fill);
    if (stream->Read(this->buf.start, 3) != 3) {
        return false;
    }
    int size = this->GetSize();
    if (size < 3) {
        return false;
    }
    if (size == 3) {
        return true;
    }
    this->buf.Resize(size - 3, &fill);
    return stream->Read(&this->buf[3], size - 3) == size - 3;
}

bool Moho::CMessage::Read(gpg::Stream *stream) {
    if (! this->HasReadLength()) {
        if (this->buf.Size() == 0) {
            char fill = 0;
            this->buf.Resize(3, &fill);
        }
        this->pos += stream->ReadNonBlocking(&this->buf[this->pos], 3 - this->pos);
        if (! this->HasReadLength()) {
            return false;
        }
    }
    int newSize = this->GetSize();
    if (newSize < 3) {
        return false;
    }
    if (newSize == this->pos) {
        return true;
    }
    char fill = 0;
    this->buf.Resize(newSize, &fill);
    this->pos += stream->ReadNonBlocking(&this->buf[this->pos], newSize - this->pos);
    return this->pos == newSize;
}

// 0x0047C4F0
Moho::IMessageReceiver::~IMessageReceiver() {
    while (this->next != this) {
        auto linked = static_cast<Moho::SMsgReceiverLinkage *>(this->next);
        linked->dispatcher->RemoveLinkage(linked);
    }
}

// 0x0047C360
void Moho::CMessageDispatcher::PushReceiver(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec) {
    auto linkage = new Moho::SMsgReceiverLinkage{lower, upper, rec, this};
    linkage->Moho::TDatListItem<Moho::SMsgReceiverLinkage>::InsertBefore(this);
    linkage->Moho::TDatListItem<Moho::IMessageReceiver>::InsertBefore(rec);
    if (lower < upper) {
        for (int k = lower; k <= upper; ++k) {
            this->receivers[k] = rec;
        }
    }
}

// 0x0047C450
void Moho::CMessageDispatcher::RemoveLinkage(Moho::SMsgReceiverLinkage *linkage) {
    int lower = linkage->lower;
    Moho::IMessageReceiver **cur = &this->receivers[lower];
    while (lower < linkage->upper) {
        if (*cur == linkage->rec) {
            *cur = nullptr;
            for (auto i = linkage->Moho::TDatListItem<Moho::SMsgReceiverLinkage>::next; i != this; i = i->next) {
                Moho::SMsgReceiverLinkage *msg = i->Get();
                if (msg->lower <= lower && lower < msg->upper) {
                    *cur = msg->rec;
                }
            }
        }
        ++lower;
        ++cur;
    }
    delete(linkage);
}
