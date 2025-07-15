#include "PipeStream.h"

bool gpg::PipeStream::Empty() {
    return this->VirtEmpty() && this->VirtAtEnd();
}