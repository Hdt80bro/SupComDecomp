#include "gpgcore/String.h"

namespace Moho {

bool FILE_IsLocal(gpg::StrArg filename); // 0x0040FA50
bool FILE_IsAbsolute(gpg::StrArg filename); // 0x0040FD60
bool FILE_HasDrive(gpg::StrArg filename); // 0x0040FDC0
bool FILE_HasUNC(gpg::StrArg filename); // 0x0040FEB0
std::string FILE_MakeAbsolute(gpg::StrArg dir, gpg::StrArg filename); // 0x0040FFC0
char const *FILE_Ext(gpg::StrArg filename); // 0x00410650
std::string FILE_SuggestedExt(gpg::StrArg dir, gpg::StrArg filename); // 0x00410760
std::string FILE_ForcedExt(gpg::StrArg dir, gpg::StrArg filename); // 0x004108B0
std::string FILE_DirPrefix(gpg::StrArg filename); // 0x00410A10
std::string FILE_Dir(gpg::StrArg filename); // 0x00410C60
std::string FILE_Base(gpg::StrArg filename, bool removeExt); // 0x004111C0
bool FILE_Wild(gpg::StrArg dir, gpg::StrArg pattern, bool, char); // 0x00411320
std::string FILE_CollapsePath(__in gpg::StrArg filename, __out bool *success); // 0x004115A0
std::string FILE_GetErrorFromErrno(int err); // 0x00411A20

bool URI_Split(__in gpg::StrArg uri, __out std::string &scheme, __out std::string &authority, __out std::string &path, __out std::string &query, __out std::string &fragment); // 0x0048E0C0

}

BOOL func_FileWildMatch(const char *path, const char *pat); // 0x00411500
