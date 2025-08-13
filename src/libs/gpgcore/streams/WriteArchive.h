#include <map>
#include <string>

namespace gpg {

// 0x00D48DDC
class WriteArchive
{
public:
    std::map<unk_t> mMap1;
    std::map<unk_t> mMap2;

    virtual ~WriteArchive() = default; // 0x00953C80
    virtual void WriteBytes(char *, size_t) = 0;
    virtual void WriteString(std::string *) = 0;
    virtual void WriteFloat(float) = 0;
    virtual void WriteUInt64(unsigned __int64) = 0;
    virtual void WriteInt64(__int64) = 0;
    virtual void WriteULong(unsigned long) = 0;
    virtual void WriteLong(long) = 0;
    virtual void WriteUInt(unsigned int) = 0;
    virtual void WriteInt(int) = 0;
    virtual void WriteUShort(unsigned short) = 0;
    virtual void WriteShort(short) = 0;
    virtual void WriteUByte(unsigned __int8) = 0;
    virtual void WriteByte(__int8) = 0;
    virtual void WriteBool(bool) = 0;
    virtual void EndSection() = 0;
    virtual void Close() = 0;
    virtual void WriteMarker(int) = 0;
};

}
