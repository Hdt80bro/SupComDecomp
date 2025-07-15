#include <string>
#include <vector>
#include <Windows.h>

static std::string whitespaceChars;

namespace gpg {

struct StrArg
{
    const char *str;
};

int STR_Utf8ByteOffset(const char *str, int pos); // 0x009380A0
int STR_Utf8Len(char const *); // 0x00938070
std::string STR_Utf8SubString(const char *str, int pos, int len); // 0x009387D0
const char *STR_NextUtf8Char(const char *str); // 0x00938040
const char *STR_PreviousUtf8Char(const char *str, const char *start); // 0x00938020
char *STR_EncodeUtf8Char(char *, wchar_t, const char *); // 0x00937F90
const char *STR_DecodeUtf8Char(const char *, wchar_t &); // 0x00937EF0
std::string STR_WideToUtf8(const wchar_t *); // 0x00938680
std::wstring STR_Utf8ToWide(const char *str); // 0x00938720

bool STR_GetToken(IN OUT const char *&find, IN const char *str, OUT std::string &dest); // 0x00938CB0
void STR_GetTokens(IN OUT const char *find, IN const char *str, OUT std::vector<std::string> &dest); // 0x00938F40
int STR_GetNextWordStartIndex(std::string &str, int pos); // 0x00938570
int STR_GetWordStartIndex(std::string &str, int pos); // 0x009384A0
bool STR_EndsWith(const char *str, const char *end); // 0x00938190
bool STR_StartsWith(char *str, char *start); // 0x00938210
bool STR_EndsWithNoCase(const char *str, const char *end); // 0x00938250
bool STR_StartsWithNoCase(const char *str, const char *start); // 0x009382B0
bool STR_IsIdent(const char *str); // 0x009382F0
int STR_Replace(std::string &str, const char *what, const char *with, unsigned int unk); // 0x00938B40
int STR_ParseUInt32(const char *str); // 0x00938150
int STR_Xtoi(const char *str); // 0x009380F0
bool STR_MatchWildcard(const char *, const char *); // 0x00938450
bool STR_WildcardValidPrefix(const char *, const char *); // 0x00938470
std::string gpg::STR_GetWhitespaceCharacters(); // 0x00938C80
std::string gpg::STR_Chop(const char *str, char chr); // 0x00938BF0
std::string STR_ToLower(const char *str); // 0x00938A80
std::string STR_ToUpper(const char *str); // 0x009389C0
std::string STR_TrimWhitespace(const char *str); // 0x009388C0
std::string STR_Printf(const char *fmt, int args...); // 0x00938F10
std::string STR_Va(const char *&fmt, va_list va); // 0x00938E00

};