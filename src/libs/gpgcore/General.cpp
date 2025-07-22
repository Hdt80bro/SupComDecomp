#include "General.h"
#include "gpgcore/String.h"


gpg::die_handler_t die_handler; // 0x00F8EBDC

// 0x00937CB0
void gpg::Logf(const char *args, ...) {
    va_list va;
    va_start(va, args);
    const char *fmt = va_arg(args, const char *);
    std::string msg = gpg::STR_Va(fmt, va);
    va_end(va);
    call_once(sub_937AD0, &dword_F8EBBC);
    if (dword_F8EBC0) {
        sub_937640(dword_F8EBC0, 1, &msg);
    }
}

// 0x00937D30
void gpg::Warnf(const char *args, ...) {
    va_list va;
    va_start(va, args);
    const char *fmt = va_arg(args, const char *);
    std::string msg = gpg::STR_Va(fmt, va);
    va_end(va);
    call_once(sub_937AD0, &dword_F8EBBC);
    if (dword_F8EBC0) {
        sub_937640(dword_F8EBC0, 1, &msg);
    }
}


// 0x0093EDE0
void gpg::HandleAssertFailure(const char *msg, int line, const char *file) {
    gpg::InvokeDieHandler(gpg::STR_Printf("Failed assertion: %s\nFile: %s\nLine: %d", msg, file, line).c_str());
}

// 0x00938FE0
gpg::die_handler_t SetDieHandler(gpg::die_handler_t handler) {
    gpg::die_handler_t old = die_handler;
    die_handler = handler;
    return old;
}

// 0x00938FF0
void gpg::InvokeDieHandler(const char *msg) {
    if (die_handler != nullptr) {
        die_handler(msg);
    }
}

// 0x00939000
void gpg::Die(const char *args, ...) {
    va_list va;
    va_start(va, args);
    const char *fmt = va_arg(args, const char *);
    std::string msg = gpg::STR_Va(fmt, va);
    va_end(va);
    gpg::InvokeDieHandler(msg.c_str());
    __debugbreak();
    while (true)
        ;
}
