#include "Exception.h"
#include "core/Plat.h"

// 0x004052A0
Moho::XException::XException(const char *msg) :
    std::runtime_error{std::string{msg}},
    topStack{0}
{
    this->topStack = Moho::PLAT_GetCallStack(nullptr, 32, this->callstack);
}


// 0x00405470
const char *Moho::XException::GetMessage() {
    if (this->msg.size() == 0) {
        this->msg = this->what();
        if (this->topStack != 0) {
            this->msg.append("\n\nOriginal callstack:\n\n");
            this->msg.append(Moho::PLAT_FormatCallstack(0, this->topStack, this->callstack));
        }
    }
    return this->msg.c_str();
}
