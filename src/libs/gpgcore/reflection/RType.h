
namespace gpg {

    class RType : RObject
    {
        bool mFinished;
        bool mInitFinished;
        int size;
        int mVersion;
        int mSerSaveConstructArgsFunc;
        void (__cdecl *mSerSaveFunc)(BinaryWriteArchive *, int, _DWORD, int);
        int mNew;
        void (__cdecl *mSerLoadFunc)(gpg::ReadArchive *, int, int, gpg::RRef *);
        int v8;
        int v9;
        std::vector<RField> bases;
        std::vector<RField> mFields;
        gpg::RRef *(__cdecl *NewRef)(gpg::RRef *);
        int CpyRef;
        void (__cdecl *mDelete)(void *);
        int CtrRef;
        int MovRef;
        int Dtr;
        bool v24;
    };

};