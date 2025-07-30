#include <stdexcept>

#define GPGGAL_THROW(msg) throw gpg::gal::Error{std::string{__FILE__}, __LINE__, std::string{msg}}

namespace gpg::gal {

// 0x00D42104
class Error : public std::exception
{
public:
    std::string file;
    int line;
    std::string msg;

    ~Error() override = default; // 0x008A7B10
    const char *what() const override {
        return this->msg.c_str();
    } // 0x00940460

    Error(std::string &file, int line, std::string &msg) :
        file(file),
        line{line},
        msg{msg}
    {} // 0x009404D0
};

}
