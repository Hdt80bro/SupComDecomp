#include "MemBuffer.h"

// 0x0094E320
gpg::MemBuffer<char> gpg::AllocMemBuffer(size_t size) {
    char *buff = (char *) malloc(size);
    memset(buff, 0, size);
    boost::shared_ptr<char> ptr{buff, free};
    return gpg::MemBuffer<char>{ptr, buff, &buff[size]};
}
