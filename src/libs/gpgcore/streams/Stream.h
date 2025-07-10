#include <stdexcept>

namespace gpg {

    class Stream
    {
    public:

        class UnsupportedOperation : std::logic_error
        {
        public:
            UnsupportedOperation(); // 0x00956E40
        };

        enum class Mode
        {
            
        };
        enum class SeekOrigin
        {
            
        };
        
        char *start;
        char *readHead;
        char *end;
        char *writeHead;
        char *writeStart;
        char *dataEnd;

        virtual ~Stream(); // 0x00956E20
        virtual size_t VirtTell(gpg::Stream::Mode mode); // 0x00956F50
        virtual size_t VirtSeek(gpg::Stream::Mode mode, gpg::Stream::SeekOrigin orig, size_t); // 0x00956F90
        virtual unsigned int VirtRead(char * buf, unsigned int len); // 0x00956FB0
        virtual unsigned int VirtReadNonBlocking(char * buf, unsigned int len); // 0x00956DE0
        virtual void VirtUnGetByte(int); // 0x00956FD0
        virtual bool VirtAtEnd(); // 00956DF0
        virtual void VirtWrite(const char *data, unsigned int size); // 0x00956FF0
        virtual void VirtFlush(); // 0x00956E00
        virtual void VirtClose(gpg::Stream::Mode mode); // 0x00956E10

        Stream() = default; // 0x00956DB0
    };

};