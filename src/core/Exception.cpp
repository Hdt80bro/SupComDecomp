#include "Exception.h"
#include "core/Plat.h"


// 0x00405470
const char *Moho::XException::what() const {
    if (this->mMsg.size() == 0) {
        this->mMsg = this->std::runtime_error::what();
        if (this->mTopStack != 0) {
            this->mMsg.append("\n\nOriginal callstack:\n\n");
            this->mMsg.append(Moho::PLAT_FormatCallstack(0, this->mTopStack, this->mCallstack));
        }
    }
    return this->mMsg.c_str();
}

// 0x004052A0
Moho::XException::XException(const char *msg) :
    std::runtime_error{std::string{msg}},
    mTopStack{0},
    mMsg{}
{
    this->mTopStack = Moho::PLAT_GetCallStack(nullptr, 32, this->mCallstack);
}


// 0x00405390
Moho::XException::XException(const char *msg, void *stack, unsigned int size) :
   std::runtime_error{msg},
   mTopStack{size},
   mMsg{}
{
    if (size > 0) {
        memcpy(this->mCallstack, stack, 4 * min(size, 32));
    }
}
