#include <vector>
#include "gpgcore/reflection/RField.hpp"
#include "gpgcore/reflection/RIndexed.hpp"
#include "gpgcore/reflection/RObject.hpp"
#include "gpgcore/reflection/RRef.hpp"
#include "gpgcore/streams/WriteArchive.hpp"
#include "gpgcore/streams/ReadArchive.hpp"

namespace gpg {

class REnumType;

// 0x00D41694
class RType : public gpg::RObject
{
public:
    bool mFinished;
    bool mInitFinished;
    int mSize;
    int mVersion;
    int mSerSaveConstructArgsFunc;
    void (__cdecl *mSerSaveFunc)(gpg::WriteArchive *, int, int, int);
    int mNew;
    void (__cdecl *mSerLoadFunc)(gpg::ReadArchive *, int, int, gpg::RRef *);
    int v8;
    int v9;
    std::vector<gpg::RField> mBases;
    std::vector<gpg::RField> mFields;
    gpg::RRef *(*NewRef)(gpg::RRef *);
    int mCpyRef;
    void (*mDelete)(void *);
    int mCtrRef;
    int mMovRef;
    int mDtr;
    bool v24;


    virtual const char *GetName() = 0;
    virtual std::string GetLexical(const gpg::RRef &) const;
    virtual bool SetLexical(const gpg::RRef &, const char *) const;
    virtual const gpg::RIndexed *IsIndexed() const;
    virtual const gpg::RIndexed *IsPointer() const;
    virtual const gpg::REnumType *IsEnumType() const;
    virtual void Init();
    virtual void Finish();
};

}
