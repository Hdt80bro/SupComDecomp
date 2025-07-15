#include <stdexcept>
#include "String.h"


std::string whitespaceChars{" \n\t\r"};

std::string gpg::STR_GetWhitespaceCharacters() {
    return std::string{whitespaceChars};
}


int gpg::STR_Utf8ByteOffset(const char *str, int pos) {
    const char *itr = str;
    int i;
    for (i = 0; *itr; ++i) {
        if (i >= pos) {
            break;
        }
        itr = gpg::STR_NextUtf8Char(itr);
    }
    return itr - str;
}

int gpg::STR_Utf8Len(char const *str) {
    int i;
    for (i = 0; *str; ++i) {
        str = gpg::STR_NextUtf8Char(str);
    }
    return i;
}

const char *gpg::STR_NextUtf8Char(const char *str) {
    if (*str) {
        char c;
        do {
            c = *++str;
        } while (c && (c & 0xC0) == 0x80);
    }
    return str;
}

const char *gpg::STR_PreviousUtf8Char(const char *str, const char *start) {
    char c;
    do {
        if (start != nullptr && str <= start) {
            break;
        }
        c = *--str;
    } while ((c & 0xC0) == 0x80);
    return str;
}

std::string gpg::STR_Utf8SubString(const char *str, int pos, int len) {
    const char *start = nullptr;
    for (int i = 0; ; ++i) {
        if (i == pos) {
            start = str;
        }
        if (i == pos + len) {
            if (str != nullptr) {
                return std::string{start, str - start};
            }
            break;
        }
        if (! *str) {
            break;
        }
        str = gpg::STR_NextUtf8Char(str);
    }
    if (start == nullptr) {
        throw std::out_of_range{std::string{"offset past end of string"}};
    }
    return std::string{start, str - start};
}

char *gpg::STR_EncodeUtf8Char(char *dest, wchar_t chr, const char *limit) {
    if (chr >= 0x80) {
        if (chr >= 0x800) {
            if (! limit || dest + 3 <= limit) {
                dest[0] = (chr >> 12) | 0xE0;
                dest[1] = (chr >> 6) & 0x3F | 0x80;
                dest[2] = chr & 0x3F | 0x80;
                return dest + 3;
            }
        } else if (! limit || dest + 2 <= limit) {
            dest[0] = (chr >> 6) | 0xC0;
            dest[1] = chr & 0x3F | 0x80;
            return dest + 2;
        }
    } else if (! limit || dest + 1 <= limit) {
        dest[0] = chr;
        return dest + 1;
    }
    return dest;
}
const char *gpg::STR_DecodeUtf8Char(const char *str, wchar_t &dest) {
    char c1 = *str;
    ++str;
    wchar_t chr = c1;
    if (c1 >= 0) {
        dest = chr;
        return str;
    }
    if ((c1 & 0xE0) == 0xC0) {
        chr = (c1 & 0x1F) << 6;
        char c2 = *str;
        if ((c2 & 0xC0) == 0x80) {
            chr |= c2 & 0x3F;
            ++str;
        }
    } else {
        if ((c1 & 0xF0) == 0xE0) {
            chr = c1 << 12;
        } else if ((c1 & 0xF8) == 0xF0) {
            char c2 = *str;
            if ((c2 & 0xC0) == 0x80) {
                chr = c2 << 12;
                ++str;
            } else {
                dest = 0;
                return str;
            }
        } else {
            dest = chr;
            return str;
        }
        char nextC = *str;
        if ((nextC & 0xC0) == 0x80) {
            chr |= (nextC & 0x3F) << 6;
            ++str;
            nextC = *str;
            if ((nextC & 0xC0) == 0x80) {
                chr |= nextC & 0x3F;
                ++ str;
            }
        }
    }
    dest = chr;
    return str;
}

std::string gpg::STR_WideToUtf8(const wchar_t *str) {
    std::string builder{};
    if (str != nullptr) {
        char buff[4];
        for (wchar_t c = *str; c; c = *++str) {
            char *end = gpg::STR_EncodeUtf8Char(buff, c, &buff[sizeof(buff)]);
            builder.append(buff, end - buff);
        }
    }
    return builder;
}

std::wstring gpg::STR_Utf8ToWide(const char *str) {
    std::wstring builder{};
    if (str == nullptr) {
        return builder;
    }
    wchar_t c;
    for (str = gpg::STR_DecodeUtf8Char(str, c); c; str = gpg::STR_DecodeUtf8Char(str, c)) {
        builder.append(1, c);
    }
    return builder;
}

bool gpg::STR_GetToken(const char *&find, const char *str, std::string &dest) {
    int c = *find;
    while (c && strchr(str, c) != 0) {
        c = *++find;
    }
    if (c) {
        const char *start = find;
        c = *++find;
        while (c && strchr(str, c) != 0) {
            c = *++find;
        }
        dest = std::string{start, find};
        if (c) {
            ++find;
        }
        return true;
    }
    dest.clear();
    find = nullptr;
    return false;
}


void gpg::STR_GetTokens(const char *find, const char *str, std::vector<std::string> &vec) {
    std::string token{};
    while (gpg::STR_GetToken(find, str, token)) {
        vec.push_back(token);
    }
}

int gpg::STR_Replace(std::string &str, const char *what, const char *with, unsigned int unk) {
    int n = 0;
    if (unk) {
        unsigned int searchPos = 0;
        int pos;
        while ((pos = str.find(what, searchPos, strlen(what))) != std::string::npos) {
            str.replace(pos, strlen(what), with);
            searchPos = pos + strlen(with);
            ++n;
        }
    }
    return n;
}

bool gpg::STR_EndsWith(const char *str, const char *end) {
    unsigned int strLen = strlen(str);
    unsigned int endLen = strlen(end);
    return strLen > endLen && ! strcmp(&str[strLen - endLen], end);
}

bool gpg::STR_StartsWith(char *str, char *start) {
  return strncmp(str, start, strlen(start)) == 0;
}

bool gpg::STR_EndsWithNoCase(const char *str, const char *end) {
    unsigned int strLen = strlen(str);
    unsigned int endLen = strlen(end);
    return strLen > endLen && ! stricmp(&str[strLen - endLen], end);
}

bool gpg::STR_StartsWithNoCase(const char *str, const char *start) {
  return strnicmp(str, start, strlen(start)) == 0;
}

bool gpg::STR_IsIdent(const char *str) {
    char c = *str++;
    if (c == '\0' || (c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && c != '_') {
        return false;
    }
    c = *str++;
    while (c) {
        if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9') && c != '_') {
            return false;
        }
        c = *str++;
    }
    return true;
}

int gpg::STR_GetWordStartIndex(std::string &str, int pos) {
    if (gpg::STR_Utf8Len(str.data()) <= 1) {
        return 0;
    }
    bool boundary = false;
    for (const char *i = &str[gpg::STR_Utf8ByteOffset(str.data(), pos)];
        *i; i = gpg::STR_NextUtf8Char(i)
    ) {
        char c = *i;
        int res = whitespaceChars.find(&c, 0, 1);
        if (! boundary) {
            boundary = res == std::string::npos;
        } else if (res != std::string::npos) {
            return pos;
        }
        --pos;
    }
    return pos;
}

int gpg::STR_GetNextWordStartIndex(std::string &str, int pos) {
    int len = gpg::STR_Utf8Len(str.data());
    if (len == 0 || len <= pos) {
        return len;
    }
    bool boundary = false;
    for (const char *i = &str[gpg::STR_Utf8ByteOffset(str.data(), pos)];
        *i; i = gpg::STR_NextUtf8Char(i)
    ) {
        char c = *i;
        int res = whitespaceChars.find(&c, 0, 1);
        if (! boundary) {
            boundary = res == std::string::npos;
        } else if (res == std::string::npos) {
            return pos;
        }
        ++pos;
    }
    return pos;
}

std::string gpg::STR_Chop(const char *str, char chr) {
    if (str && *str) {
        int size = strlen(str);
        if (! chr || str[size - 1] == chr) {
            --size;
        }
        return std::string{str, &str[size]};
    } else {
        return std::string{""};
    }
}

int gpg::STR_ParseUInt32(const char *str) {
    if (str == nullptr) {
        return 0;
    }
    if (strnicmp(str, "0x", 2) == 0) {
        return gpg::STR_Xtoi(&str[2]);
    }
    return atoi(str);
}

int gpg::STR_Xtoi(const char *str) {
    int res = 0;
    if (str != nullptr) {
        for (char c = *str; c != NULL; c = *++str) {
            res *= 16;
            if (c - '0' <= 9) {
                res += c - '0';
            } else if (c - 'A' <= 5) {
                res += 10 + c - 'A';
            } else if (c - 'a' <= 5) {
                res += 10 + c - 'a';
            } else {
                return res;
            }
        }
    }
    return res;
}

std::string gpg::STR_TrimWhitespace(const char *str) {
    std::string builder{};
    if (str != nullptr) {
        while (*str == ' ' || *str != '\t' || *str != '\r' && *str != '\n') {
            ++str;
        }
        builder = str;
        int size = builder.size();
        while (size) {
            char c = builder[size - 1];
            if (c != ' ' && c != '\t' && c != '\r' && c != '\t') {
                break;
            }
            --size;
        }
        builder.resize(size);
    }
    return builder;
}

std::string gpg::STR_ToLower(const char *str) {
    std::string builder{};
    builder.reserve(strlen(str));
    for (char c = *str; c != NULL; c = *++str) {
        if (c - 'A' <= 25) {
            c += 'a' - 'A';
        }
        builder.append(1, c);
    }
    return builder;
}

std::string gpg::STR_ToUpper(const char *str) {
    std::string builder{};
    builder.reserve(strlen(str));
    for (char c = *str; c != NULL; c = *++str) {
        if (c - 'a' <= 25) {
            c -= 'a' - 'A';
        }
        builder.append(1, c);
    }
    return builder;
}

std::string gpg::STR_Printf(const char *fmt, int args...) {
    va_list va;
    va_start(va, args);
    gpg::STR_Va(fmt, va);
    va_end(va);
    return fmt;
}

std::string gpg::STR_Va(const char *&fmt, va_list va) {
    std::string builder{};
    char buffer[256];
    int res = vsnprintf(buffer, sizeof(buffer), fmt, va);
    if (res >= 0) {
        builder.append(buffer, res);
    } else {
        std::vector<char> dynBuf;
        int size = 256;
        do {
            size *= 2;
            dynBuf.resize(size, 0);
            res = vsnprintf(dynBuf.data(), size, fmt, va);
        } while (res == -1);
        builder.append(dynBuf.data(), res);
    }
    return builder;
}
