#include "PipeStream.h"

// 0x009565D0
gpg::PipeStream::PipeStream() :
    gpg::Stream{},
    lock{},
    semaphore{},
    buf{}
{
    auto b = new struct_stream_buffer{};
    b->InsertBefore(&this->buf);
    this->end = b->data;
    this->readHead = b->data;
    this->start = b->data;
    this->writeStart = b->data;
    this->writeHead = b->data;
    this->dataEnd = &b->data[sizeof(b->data)];
}

// 0x009569A0
gpg::PipeStream::~PipeStream() {
    while (this->buf.next != &this->buf) {
        auto cur = this->buf.next;
        if (cur) {
            delete(cur); // dtr moves next buffer to head
        }
    }
}

// 0x00483470
bool gpg::PipeStream::Empty() {
    return this->readHead == this->end && this->VirtAtEnd();
}

// 0x009566C0
unsigned int gpg::PipeStream::GetLength() {
    boost::mutex::scoped_lock lock{this->lock};
    unsigned int i = this->writeHead - this->readHead;
    if (this->buf.next != this->buf.prev) {
        i = &this->end[i] - (char *)this->buf.prev - 8;
        for (auto cur = this->buf.next->next; cur != this->buf.prev; cur = cur->next) {
            i += 4096;
        }
    }
    return i;
}
