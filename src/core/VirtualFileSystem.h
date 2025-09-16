#include <string>
#include <vector>


namespace Moho {

struct SVFSMountPoint
{
    std::string mDir;
    std::string mMountpoint;
    bool mIsZip;
};

struct SDiskFileInfo
{
    int v0;
    int v1;
    int v2;
    int v3;
};


class CVirtualFileSystem
{
public:
    virtual ~CVirtualFileSystem() = default; // 0x004661D0
    virtual std::string FindFile(__in const char *filename, __out Moho::SDiskFileInfo *info) = 0;
    virtual std::string Func1(const char *) = 0;
    virtual bool Func2(__in const char *, __out Moho::SDiskFileInfo *) = 0;
    virtual void Func3(const char *, bool, bool, std::vector<std::string> *) = 0;
    virtual void Func4(const char *, std::vector<std::string> *) = 0;
};

class CVFSImpl : public Moho::CVirtualFileSystem
{
public:
    std::vector<Moho::SVFSMountPoint> mMountPoints;
    
    ~CVFSImpl() override = default; // 0x004662F0
    std::string FindFile(__in const char *filename, __out Moho::SDiskFileInfo *info) override; // 0x00466A20
    std::string Func1(const char *) override; // 0x
    bool Func2(__in const char *, __out Moho::SDiskFileInfo *) override; // 0x00466E50
    void Func3(const char *, bool, bool, std::vector<std::string> *) override; // 0x00467020
    void Func4(const char *, std::vector<std::string> *) override; // 0x00467900
};

}
