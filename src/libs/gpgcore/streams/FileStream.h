#include "Stream.h"
#include "boost/weak_ptr.hpp"

namespace gpg {

class FileStream : public gpg::Stream
{
public:
    HANDLE handle;
    DWORD accessKind;
    boost::shared_ptr<int> buffer;
    DWORD v4;
    DWORD v5;

    class IOError : public std::runtime_error
    {
    public:
        int id;

        IOError(int id); // 0x00955890
    };

    ~FileStream() override; // 0x00955870
    void VirtTell() override;
    void VirtSeek() override;
    void VirtRead() override;
    int VirtAtEnd() override;
    int VirtWrite() override;
    void VirtFlush() override;
    void VirtClose() override;


    FileStream(const char *filename, int accessKind, unsigned int attributes, int buffSize); // 0x00955BD0
    void OpenFile(const char *file, int accessKind, unsigned int attributes); // 0x00955990
};

std::string FileErrorToString(int id); // 0x00957950

};