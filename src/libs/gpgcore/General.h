
#define GPG_ASSERT(x) do {\
    if (! (x)) {\
        gpg::HandleAssertFailure(#x, __LINE__, __FILE__);\
    }\
} while (false)


namespace gpg {

using die_handler_t = void (*)(const char *);

}

static gpg::die_handler_t die_handler; // 0x00F8EBDC

namespace gpg {

void Logf(const char *args, ...); // 0x00937CB0
void Warnf(const char *args, ...); // 0x00937D30
void HandleAssertFailure(const char *msg, int lineNum, const char *file); // 0x0093EDE0
gpg::die_handler_t SetDieHandler(gpg::die_handler_t handler); // 0x00938FE0
void InvokeDieHandler(const char *); // 0x00938FF0
void Die(const char *args, ...); // 0x00939000

}
