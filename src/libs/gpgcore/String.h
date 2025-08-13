#include <string>
#include <vector>
#include <Windows.h>

static std::string sWhitespaceChars; // 0x00F32308

namespace gpg {

using StrArg = const char *;
using WStrArg = const wchar_t *;

int STR_Utf8ByteOffset(gpg::StrArg str, int pos); // 0x009380A0
int STR_Utf8Len(char const *); // 0x00938070
const char *STR_NextUtf8Char(const char *str); // 0x00938040
const char *STR_PreviousUtf8Char(const char *str, const char *start); // 0x00938020
std::string STR_Utf8SubString(gpg::StrArg str, int pos, int len); // 0x009387D0
char *STR_EncodeUtf8Char(char *, wchar_t, const char *); // 0x00937F90
const char *STR_DecodeUtf8Char(const char *, wchar_t &); // 0x00937EF0
std::string STR_WideToUtf8(const wchar_t *); // 0x00938680
std::wstring STR_Utf8ToWide(gpg::StrArg str); // 0x00938720

bool STR_GetToken(IN OUT const char *&find, IN const char *str, OUT std::string &dest); // 0x00938CB0
void STR_GetTokens(IN OUT gpg::StrArg find, IN const char *str, OUT std::vector<std::string> &dest); // 0x00938F40
int STR_GetWordStartIndex(std::string &str, int pos); // 0x009384A0
int STR_GetNextWordStartIndex(std::string &str, int pos); // 0x00938570
bool STR_EndsWith(gpg::StrArg str, gpg::StrArg end); // 0x00938190
bool STR_StartsWith(gpg::StrArg str, gpg::StrArg start); // 0x00938210
bool STR_EndsWithNoCase(gpg::StrArg str, gpg::StrArg end); // 0x00938250
bool STR_StartsWithNoCase(gpg::StrArg str, gpg::StrArg start); // 0x009382B0
bool STR_IsIdent(gpg::StrArg str); // 0x009382F0
int STR_Replace(std::string &str, gpg::StrArg what, gpg::StrArg with, unsigned int unk); // 0x00938B40
int STR_ParseUInt32(gpg::StrArg str); // 0x00938150
int STR_Xtoi(gpg::StrArg str); // 0x009380F0
bool STR_MatchWildcard(gpg::StrArg, gpg::StrArg); // 0x00938450
bool STR_WildcardValidPrefix(gpg::StrArg, gpg::StrArg); // 0x00938470
std::string gpg::STR_GetWhitespaceCharacters(); // 0x00938C80
std::string gpg::STR_Chop(gpg::StrArg str, char chr); // 0x00938BF0
std::string STR_ToLower(gpg::StrArg str); // 0x00938A80
std::string STR_ToUpper(gpg::StrArg str); // 0x009389C0
std::string STR_TrimWhitespace(gpg::StrArg str); // 0x009388C0
std::string STR_Printf(const char *args...); // 0x00938F10
std::string STR_Va(const char *&fmt, va_list va); // 0x00938E00

}
