#include <stdexcept>
#include <string>

namespace Moho {

// 0x00E00020
class XException : public std::runtime_error
{
public:
    unsigned int mCallstack[32];
    unsigned int mTopStack;
    std::string mMsg;

    ~XException() override = default; // 0x00405370
    const char *what() const override; // 0x00405470

    XException(const char *); // 0x004052A0
    XException(const char *msg, void *stack, unsigned int size); // 0x00405390
};

// 0x00E07DC0
class XDataError : public Moho::XException
{
public:
    XDataError(const char *msg) : XException{msg} {}
};

// 0x00E008D8
class XFileError : public Moho::XException
{
public:
    XFileError(const char *msg) : Moho::XException{msg} {}
    XFileError(std::string *msg, void *stack, unsigned int size) : Moho::XException{msg->c_str(), stack, size} {} // 0x0040FA80
};

#define THROW_FILE_ERROR(method, err) do {\
    unsigned int stack1[32];\
    unsigned int size = Moho::PLAT_GetCallStack(nullptr, 32, stack1);\
    std::string msg = gpg::STR_Printf("%s: %s", method, err);\
    int stack2[32];\
    throw Moho::XFileError{&msg, stack2, size};\
} while (false)

#define THROW_NULL_ARG_FILE_ERROR(method) THROW_FILE_ERROR(method, "Null argument.")

}
