#include "BinaryReader.h"

// 0x0043D210
void gpg::BinaryReader::Read(void *data, size_t size) {
    if (size > this->mStream->LeftToRead()) {
        if (this->mStream->VirtRead((char *) data, size) != size) {
            throw gpg::BinaryReader::PrematureEOF{};
        }
    } else if (size != 0) {
        memcpy(data, this->mStream->mReadPtr, size);
        this->mStream->mReadPtr += size;
    }
}
