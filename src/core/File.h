#include "gpgcore/String.h"

namespace Moho {

bool FILE_IsLocal(gpg::StrArg); // 0x0040FA50
bool FILE_IsAbsolute(gpg::StrArg); // 0x0040FD60
bool FILE_HasDrive(gpg::StrArg); // 0x0040FDC0
bool FILE_HasUNC(gpg::StrArg); // 0x0040FEB0
std::string FILE_MakeAbsolute(gpg::StrArg, gpg::StrArg); // 0x0040FFC0
char const *FILE_Ext(gpg::StrArg); // 0x00410650
std::string FILE_SuggestedExt(gpg::StrArg, gpg::StrArg); // 0x00410760
std::string FILE_ForcedExt(gpg::StrArg, gpg::StrArg); // 0x004108B0
std::string FILE_DirPrefix(gpg::StrArg, bool); // 0x00410A10
std::string FILE_Dir(gpg::StrArg); // 0x00410C60
std::string FILE_Base(gpg::StrArg, bool); // 0x004111C0
bool FILE_Wild(gpg::StrArg, gpg::StrArg, bool, char); // 0x00411320
std::string FILE_CollapsePath(gpg::StrArg, bool *); // 0x004115A0
std::string FILE_GetErrorFromErrno(int); // 0x00411A20

}
