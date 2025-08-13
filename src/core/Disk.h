
#include "core/DiskWatch.h"
#include "core/VirtualFileSystem.h"
#include "core/WaitHandleSet.h"

#include "gpgcore/streams/Stream.h"
#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"
#include "boost/filesystem/path.hpp"
#include "boost/thread/once.hpp"
#include <memory>


static boost::once_flag once_FileWaitSet; // 0x010A6370
static Moho::FWaitHandleSet *sPWaitHandleSet; // 0x010A6374
static boost::once_flag once_InitDiskWatch; // 0x010A6378
static Moho::CDiskWatch *sDiskWatch; // 0x010A637C
static Moho::FWaitHandleSet sWaitHandleSet; // 0x01103B30

namespace Moho {

Moho::CDiskMountedZipHandle DISK_MountZipFile(gpg::StrArg); // 0x00459AF0
bool DISK_GetFileInfo(gpg::StrArg, Moho::SDiskFileInfo *); // 0x00459B30
void DISK_InvalidateFileInfoCache(gpg::StrArg); // 0x00459B60
std::auto_ptr<gpg::Stream> DISK_OpenFileWrite(gpg::StrArg); // 0x00459B90
std::auto_ptr<gpg::Stream> DISK_OpenFileRead(gpg::StrArg); // 0x00459C40
gpg::MemBuffer<char> DISK_ReadFile(gpg::StrArg); // 0x00459CD0
gpg::MemBuffer<char const> DISK_MemoryMapFile(gpg::StrArg); // 0x00459D10
std::string DISK_GetLastError(); // 0x00459D50
std::vector<std::wstring> DISK_GetAllowedProtocols(); // 0x00459DA0
Moho::CVirtualFileSystem *DISK_GetVFS(); // 0x00459DC0
bool DISK_SetupDataAndSearchPaths(std::string, const boost::filesystem::path &); // 0x00459DE0
boost::filesystem::path DISK_GetLaunchDir(); // 0x0045A670
bool DISK_CreateFolder(gpg::StrArg); // 0x0045A770
void DISK_Recycle(gpg::StrArg); // 0x0045A7A0
bool DISK_AddWatchDirectory(gpg::StrArg); // 0x004632B0
bool DISK_RemoveWatchDirectory(gpg::StrArg); // 0x004632E0
void DISK_AddWatchListener(Moho::CDiskWatchListener *); // 0x00463310
void DISK_RemoveWatchListener(Moho::CDiskWatchListener *); // 0x00463340
void DISK_UpdateWatcher(); // 0x004633A0
void DISK_ResetWatch(); // 0x004633D0

}

Moho::FWaitHandleSet *func_GetFWaitHandleSet(); // 0x00457ED0 always inlined
void func_EnsureFileCWaitHandleSet(); // 0x00457F90
void func_InitDiskWatch(); // 0x00463220
