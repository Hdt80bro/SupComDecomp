
namespace gpg {

// 0x00D48D14
class ReadArchive
{
public:
    virtual ~ReadArchive() = default; // 0x00953700
    virtual void ReadBytes(char *, size_t) = 0;
    virtual void ReadString(std::string *) = 0;
    virtual void ReadFloat(float *) = 0;
    virtual void ReadUInt64(unsigned __int64 *) = 0;
    virtual void ReadInt64(__int64 *) = 0;
    virtual void ReadULong(unsigned long *) = 0;
    virtual void ReadLong(long *) = 0;
    virtual void ReadUInt(unsigned int *) = 0;
    virtual void ReadInt(int *) = 0;
    virtual void ReadUShort(unsigned short *) = 0;
    virtual void ReadShort(short *) = 0;
    virtual void ReadUByte(unsigned __int8 *) = 0;
    virtual void ReadByte(__int8 *) = 0;
    virtual void ReadBool(bool *) = 0;
    virtual void EndSection() = 0;
    virtual int NextMarker() = 0;
};

}
