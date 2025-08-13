#include "Disk.h"
#include "gpgcore/streams/FileStream.h"


boost::once_flag once_FileWaitSet = BOOST_ONCE_INIT; // 0x010A6370
Moho::FWaitHandleSet *sPWaitHandleSet; // 0x010A6374
boost::once_flag once_InitDiskWatch = BOOST_ONCE_INIT; // 0x010A6378
Moho::CDiskWatch *sDiskWatch; // 0x010A637C
Moho::FWaitHandleSet sWaitHandleSet; // 0x01103B30


inline void func_CheckDiskWatch() {
    if (sDiskWatch == nullptr) {
        boost::call_once(func_InitDiskWatch, &once_InitDiskWatch);
    }
}


// 0x00459B90
std::auto_ptr<gpg::Stream> *Moho::DISK_OpenFileWrite(gpg::StrArg filename) {
    return std::auto_ptr<gpg::Stream>{
        new gpg::FileStream{filename, gpg::Stream::ModeSend, 0, 4096}
    };
}

// 0x0045A7A0
void Moho::DISK_Recycle(gpg::StrArg filename) {
    SHFILEOPSTRUCTW fileOp;
    fileOp.hwnd = 0;
    fileOp.fFlags = FOF_NOERRORUI|FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_SILENT;
    fileOp.wFunc = FO_DELETE;
    fileOp.pTo = 0;
    std::wstring wide = gpg::STR_Utf8ToWide(filename);
    wchar_t buf[262];
    wchar_t *i = buf;
    for (const wchar_t *k = wide.c_str(); *k; ++k, ++i) {
        *i = *k;
    }
    buf[wcslen(buf) + 1] = '\0';
    fileOp.pFrom = buf;
    SHFileOperationW(&fileOp);
}

// 0x004632B0
bool Moho::DISK_AddWatchDirectory(gpg::StrArg file) {
    func_CheckDiskWatch();
    sDiskWatch->AddDirectory(file);
}

// 0x004632E0
bool Moho::DISK_RemoveWatchDirectory(gpg::StrArg file) {
    func_CheckDiskWatch();
    sDiskWatch->RemoveDirectory/*RemoveDirectoryW*/(file);
}

// 0x00463310
void Moho::DISK_AddWatchListener(Moho::CDiskWatchListener *list) {
    func_CheckDiskWatch();
    sDiskWatch->AddListener(list);
}

// 0x00463340
void Moho::DISK_RemoveWatchListener(Moho::CDiskWatchListener *list) {
    func_CheckDiskWatch();
    sDiskWatch->RemoveListener(list);
}

// 0x004633A0
void Moho::DISK_UpdateWatcher() {
    func_CheckDiskWatch();
    sDiskWatch->WatchQuery();
}

// 0x004633D0
void __cdecl Moho::DISK_ResetWatch() {
    if (sDiskWatch != nullptr) {
        delete(sDiskWatch);
    }
    once_InitDiskWatch = BOOST_ONCE_INIT;
    sDiskWatch = nullptr;
}



// 0x00457ED0
Moho::FWaitHandleSet *func_GetFWaitHandleSet() {
    boost::call_once(func_EnsureFileCWaitHandleSet, &once_FileWaitSet);
    return sPWaitHandleSet;
}

// 0x00457F90
void func_EnsureFileCWaitHandleSet() {

}

// 0x00463220
void func_InitDiskWatch() {
    sDiskWatch = new Moho::CDiskWatch{};
    atexit(Moho::DISK_ResetWatch);
}
