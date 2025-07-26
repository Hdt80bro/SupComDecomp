#include <stdexcept>
#include <string>

namespace Moho {

class XException : public std::runtime_error
{
public:
    unsigned int callstack[32];
    unsigned int topStack;
    std::string msg;

    virtual ~XException() = default;
    virtual const char *GetMessage(); // 0x00405470

    XException(const char *); // 0x004052A0
};

class XDataError : public Moho::XException
{
public:
    XDataError(const char *msg) : XException{msg} {}
};

class XFileError : public Moho::XException
{
public:
    XFileError(const char *msg) : XException{msg} {}
};

}