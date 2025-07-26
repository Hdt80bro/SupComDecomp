#include "Disk.h"
#include "gpgcore/streams/FileStream.h"



boost::once_flag once_filewaitset; // 0x010A6370
Moho::FWaitHandleSet *pfwaithandleset; // 0x010A6374
boost::once_flag once_InitDiskWatch; // 0x010A6378
Moho::CDiskWatch *cdiskwatch; // 0x010A637C

Moho::FWaitHandleSet fwaithandleset; // 0x01103B30


inline void func_CheckDiskWatch() {
    if (cdiskwatch == nullptr) {
        call_once(func_InitDiskWatch, &once_InitDiskWatch);
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
    _SHFILEOPSTRUCTW FileOp;
    FileOp.hwnd = 0;
    FileOp.fFlags = FOF_NOERRORUI|FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_SILENT;
    FileOp.wFunc = FO_DELETE;
    FileOp.pTo = 0;
    std::wstring wide = gpg::STR_Utf8ToWide(filename);
    wchar_t buf[262];
    wchar_t *i = &buf;
    for (auto k = wide.c_str(); *k; ++k) {
        *i++ = *k;
    }
    buf[wcslen(buf) + 1] = '\0';
    FileOp.pFrom = buf;
    SHFileOperationW(&FileOp);
}

// 0x004632B0
bool Moho::DISK_AddWatchDirectory(gpg::StrArg file) {
    func_CheckDiskWatch();
    cdiskwatch.AddDirectory(list);
}

// 0x004632E0
bool Moho::DISK_RemoveWatchDirectory(gpg::StrArg file) {
    func_CheckDiskWatch();
    cdiskwatch.RemoveDirectory/*RemoveDirectoryW*/(list);
}

// 0x00463310
void Moho::DISK_AddWatchListener(Moho::CDiskWatchListener *list) {
    func_CheckDiskWatch();
    cdiskwatch.AddListener(list);
}

// 0x00463340
void Moho::DISK_RemoveWatchListener(Moho::CDiskWatchListener *list) {
    func_CheckDiskWatch();
    cdiskwatch.RemoveListener(list);
}

// 0x004633A0
void Moho::DISK_UpdateWatcher() {
    func_CheckDiskWatch();
    cdiskwatch.WatchQuery();
}

// 0x004633D0
void __cdecl Moho::DISK_ResetWatch() {
    if (cdiskwatch != nullptr) {
        delete(cdiskwatch);
    }
    once_InitDiskWatch = 0;
    cdiskwatch = nullptr;
}


// 0x00457F90
void func_EnsureFileCWaitHandleSet() {

}

// 0x00463220
void func_InitDiskWatch() {
    cdiskwatch = new Moho::CDiskWatch{};
    atexit(Moho::DISK_ResetWatch);
}
