#include "MemBuffer.h"

gpg::MemBuffer<char> gpg::AllocMemBuffer(unsigned int size) {
    char *buff = (char *) malloc(size);
    memset(buff, 0, size);
    boost::shared_ptr<char> ptr{buff, free};
    return gpg::MemBuffer<char>{ptr, buff, &buff[size]};
}
