
#define GPG_ERROR(msg) gpg::HandleAssertFailure(msg, __LINE__, __FILE__)
#define GPG_UNREACHABLE() GPG_ERROR("Reached the supposably unreachable.")
#define GPG_ASSERT(x) do {\
    if (! (x)) { GPG_ERROR(#x); }\
} while (false)


namespace gpg {

using die_handler_t = void (*)(const char *);


void Logf(const char *args, ...); // 0x00937CB0
void Warnf(const char *args, ...); // 0x00937D30
void HandleAssertFailure(const char *msg, int lineNum, const char *file); // 0x0093EDE0
gpg::die_handler_t SetDieHandler(gpg::die_handler_t handler); // 0x00938FE0
void InvokeDieHandler(const char *); // 0x00938FF0
void Die(const char *args, ...); // 0x00939000

}

static gpg::die_handler_t die_handler; // 0x00F8EBDC