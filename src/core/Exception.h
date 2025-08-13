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
    XFileError(const char *msg) : XException{msg} {}
};

}
