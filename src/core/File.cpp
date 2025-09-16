#include "File.h"
#include "core/Exception.h"
#include "core/Plat.h"
#include "Windows.h"

// 0x0040FA50
bool Moho::FILE_IsLocal(gpg::StrArg filename) {
    return
        filename != nullptr &&
        filename[0] == '/' &&
        filename[1] != ':' &&
        filename[1] != '/';
}

// 0x0040FD60
bool Moho::FILE_IsAbsolute(gpg::StrArg filename) {
    if (filename == nullptr || filename[0] == '\0') {
        return false;
    }
    if (Moho::FILE_HasUNC(filename)) {
        return true;
    }
    char c;
    if (Moho::FILE_HasDrive(filename)) {
        c = filename[2];
    } else {
        c = filename[0];
    }
    return c == '/' || c == '\\';
}

// 0x0040FDC0
bool Moho::FILE_HasDrive(gpg::StrArg filename) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_HasDrive");
    }
    char c = filename[0];
    return ('A' <= c && c <= 'Z' || c - 'a' <= 25) && filename[1] == ':';
}

// 0x0040FEB0
bool Moho::FILE_HasUNC(gpg::StrArg filename) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_HasUNC");
    }
    char c = filename[1];
    if (c != '\0') {
        return filename[0] == '/' && c == '/' || filename[0] == '\\' && c == '\\';
    }
    return c;
}

// 0x0040FFC0
std::string Moho::FILE_MakeAbsolute(gpg::StrArg dir, gpg::StrArg filename) {
    if (dir == nullptr || dir[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_MakeAbsolute");
    }
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_MakeAbsolute");
    }
    std::string dirStr{dir};
    std::string fileStr{filename};
    gpg::STR_Replace(dirStr, "\\", "/", -1);
    gpg::STR_Replace(fileStr, "\\", "/", -1);
    bool fileUnc = Moho::FILE_HasUNC(fileStr.c_str());
    bool fileDrive = Moho::FILE_HasDrive(fileStr.c_str());
    bool dirUnc = Moho::FILE_HasUNC(dirStr.c_str());
    bool dirDrive = Moho::FILE_HasDrive(dirStr.c_str());
    if (! fileUnc && ! fileDrive && fileStr[0] != '/') {
        THROW_FILE_ERROR("Moho::FILE_MakeAbsolute", "base path must be absolute");
    }
    if (dirUnc && fileStr[0] == '/' && ! fileUnc) {
        THROW_FILE_ERROR("Moho::FILE_MakeAbsolute", "UNC absolute path incompatible with posix-style base");
    }
    if (! dirDrive) {
        goto LABEL_36;
    }
    if (fileStr[0] == '/') {
        if (! fileUnc) {
            THROW_FILE_ERROR("Moho::FILE_MakeAbsolute", "Path with drive letter incompatible with posix-style base");
        }
    } else  {
        LABEL_36:
        if (! fileUnc) {
            goto LABEL_42;
        }
    }
    if (dirStr[0] == '/' && ! dirUnc) {
        THROW_FILE_ERROR("Moho::FILE_MakeAbsolute", "posix-style absolute path incompatible with UNC base");
    }
LABEL_42:
    dirStr.assign(gpg::STR_Chop(dirStr.c_str(), '/'));
    fileStr.assign(gpg::STR_Chop(fileStr.c_str(), '/'));
    if (dirUnc || dirDrive) {
        return dirStr;
    } else {
        std::string builder{};
        if (fileUnc || fileDrive) {
            builder = fileStr.substr(0, 2);
            fileStr = fileStr.substr(2);
        }
        if (! Moho::FILE_IsAbsolute(dirStr.c_str())) {
            if (builder.size() == 0 && ! Moho::FILE_IsAbsolute(fileStr.c_str())) {
                builder += "/";
            }
            builder.append(fileStr);
            if (! Moho::FILE_IsAbsolute(dirStr.c_str())) {
                builder += "/";
            }
        }
        builder.append(dirStr);
        return builder;
    }
}

// 0x00410650
char const *Moho::FILE_Ext(gpg::StrArg filename) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_Ext");
    }
    const char *k = &filename[strlen(filename)];
    while (k > filename) {
        char c = *--k;
        if (c == '/' || c == '\\') {
            break;
        }
        if (c == '.') {
            return k + 1;
        }
    }
    return nullptr;
}

// 0x00410760
std::string Moho::FILE_SuggestedExt(gpg::StrArg filename, gpg::StrArg ext) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_Ext");
    }
    std::string sugg{filename};
    if (Moho::FILE_Ext(filename) == nullptr && ext != nullptr && ext[0] != '\0') {
        if (ext[0] != '.') {
            sugg.append(1, '.');
        }
        sugg.append(ext);
    }
    return sugg;
}

// 0x004108B0
std::string Moho::FILE_ForcedExt(gpg::StrArg filename, gpg::StrArg ext) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_ForcedExt");
    }
    std::string dest{};
    const char *existing = Moho::FILE_Ext(filename);
    size_t surviving;
    if (existing != nullptr) {
        surviving = existing - filename - 1;
    } else {
        surviving = strlen(filename);
    }
    dest.append(filename, surviving);
    if (ext != nullptr && ext[0] != '\0') {
        dest.append(1, '.');
        dest.append(ext);
    }
    return dest;
}

// 0x00410A10
std::string Moho::FILE_DirPrefix(gpg::StrArg filename) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_DirPrefix");
    }
    std::string fileStr{filename};
    gpg::STR_Replace(fileStr, "\\", "/", -1);
    size_t slash = fileStr.find_last_of("/", 1);
    if (slash == std::string::npos) {
        return std::string{""};
    }
    size_t dot = fileStr.find_last_of(".", 1);
    if (dot == std::string::npos || dot <= slash) {
        if (slash == fileStr.size() - 1 &&
            (! Moho::FILE_HasDrive(fileStr.c_str()) || slash != 2)
        ) {
            fileStr.assign(fileStr.substr(0, slash));
        }
        return fileStr;
    }
    int end;
    if (Moho::FILE_HasDrive(fileStr.c_str()) && slash == 2) {
        end = 3;
    } else {
        end = slash;
    }
    return fileStr.substr(0, end);
}

// 0x00410C60
std::string Moho::FILE_Dir(gpg::StrArg filename) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_Dir");
    }
    if (filename[0] == '/' && filename[1] != ':' && filename[1] != '/') {
        THROW_FILE_ERROR("Moho::FILE_Dir", "System path expected, got local.");
    }
    std::string fileStr{filename};
    gpg::STR_Replace(fileStr, "\\", "/", -1);
    bool hasDrive = Moho::FILE_HasDrive(filename);
    bool hasUNC = Moho::FILE_HasUNC(filename);
    const char *end;
    char buffer[260];
    if (hasDrive || hasUNC) {
        if (fileStr.size() <= 2) {
            if (_getdcwd(Moho::GetDrive(filename), buffer, 260) == nullptr) {
                std::string msg = gpg::STR_Printf("_getdcwd error: %s", Moho::FILE_GetErrorFromErrno(*_errno()));
                THROW_FILE_ERROR("Moho::FILE_Dir", msg.c_str());
            }
            end = buffer;
        } else {
            if (fileStr[2] == '/' || hasUNC) {
                end = filename;
            } else {
                if (_getdcwd(Moho::GetDrive(filename), buffer, 260) == nullptr) {
                    std::string msg = gpg::STR_Printf("_getdcwd error: %s", Moho::FILE_GetErrorFromErrno(*_errno()));
                    THROW_FILE_ERROR("Moho::FILE_Dir", msg.c_str());
                }
                sprintf(buffer, "%s/%s", buffer, fileStr.substr(2).c_str());
                end = buffer;
            }
        }
    } else {
        if (fileStr[0] != '/') {
            if (_getcwd(buffer, 260) == nullptr) {
                std::string msg = gpg::STR_Printf("_getcwf error: %s", Moho::FILE_GetErrorFromErrno(*_errno()));
                THROW_FILE_ERROR("Moho::FILE_Dir", msg.c_str());
            }
            sprintf(buffer, "%s/%s", buffer, fileStr.c_str());
            end = buffer;
        } else {
            std::string dirfile = gpg::STR_Printf("%c:%s", _getdrive(), fileStr.c_str());
            end = dirfile.c_str();
        }
    }
    return Moho::FILE_DirPrefix(end);
}

// 0x004111C0
std::string Moho::FILE_Base(gpg::StrArg filename, bool removeExt) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_Dir");
    }
    const char *save = filename;
    const char *itr = filename;
    do {
        if (*itr == '/' || *itr == '\\') {
            save = itr + 1;
        }
        ++itr;
    } while (*itr != '\0');
    const char *found = nullptr;
    if (removeExt) {
        for (itr = save; *itr != '\0'; ++itr) {
            if (*itr == '.') {
                found = itr;
            }
        }
    }
    size_t len;
    if (found != nullptr) {
        len = found - save;
    } else {
        len = strlen(save);
    }
    return std::string{save, len};
}

// 0x00411320
bool Moho::FILE_Wild(gpg::StrArg dir, gpg::StrArg pat, bool a3, char a4) {
    if (dir == nullptr || dir[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_Wild");
    }
    if (pat == nullptr || pat[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_Wild");
    }
    char str[264];
    char c;
    do {
        c = *dir;
        dir[str - dir] = *dir;
        ++dir;
    } while (c);
    if (a3 && ! strchr(str, '.')) {
        char v19;
        char *v10 = &v19;
        while (*++v10)
            {}
        strcpy(v10, ".");
    }
    if (! a4) {
        return func_FileWildMatch(str, pat);
    }
    char v21[264];
    strcpy(v21, pat);
    char *v12 = v21;
    while (true) {
        char *v13 = strchr(v12, a4);
        if (v13)
            *v13 = '\0';
        if (func_FileWildMatch(str, v12))
            break;
        if (v13) {
            v12 = v13 + 1;
            if (v13 != -1) {
                continue;
            }
        }
        return false;
    }
    return true;
}

// 0x004115A0
std::string Moho::FILE_CollapsePath(__in gpg::StrArg filename, __out bool *success) {
    if (filename == nullptr || filename[0] == '\0') {
        THROW_NULL_ARG_FILE_ERROR("Moho::FILE_CollapsePath");
    }
    std::string fileStr{filename};
    gpg::STR_Replace(fileStr, "\\", "/", -1);
    // ...
}

// 0x00411A20
std::string Moho::FILE_GetErrorFromErrno(int err) {
    switch (err) {
        case 2:  return std::string{"File not found"};
        case 13: return std::string{"Access denied"};
        case 17: return std::string{"Path already exists"};
        case 22: return std::string{"Invalid characters in file name"};
        case 24: return std::string{"Too many open files."};
        default: return std::string{"Unknown disk error"};
    }
}


// 0x0048E0C0
bool Moho::URI_Split(__in gpg::StrArg uri, __out std::string &scheme, __out std::string &authority, __out std::string &path, __out std::string &query, __out std::string &fragment) {
    scheme.clear();
    authority.clear();
    path.clear();
    query.clear();
    fragment.clear();

    char c = *uri++;
    if (! isalpha(c)) {
        return false;
    }
    scheme.append(1, c);
    for (c = *uri++; c != ':'; c = *uri++) {
        if (! isalnum(c) && c != '+' && c != '-' && c != '.') {
            return false;
        }
        scheme.append(1, c);
    }
    scheme.append(1, c);

    if (uri[0] == '/' && uri[1] == '/') {
        authority = "//";
        uri += 2;
        for (c = *uri++; c != '/' && c != '?' && c != '#' && c != '\0'; c = *uri++) {
            authority.append(1, c);
        }
    } else {
        c = *uri++;
    }

    for ( ; c != '?' && c != '#' && c != '\0' ; c = *uri++) {
        path.append(1, c);
    }

    if (c == '?') {
        for ( ; c != '#' && c != '\0'; c = *uri++) {
            query.append(1, c);
        }
    }
    
    if (c == '#') {
        for ( ; c != '\0'; c = *uri++) {
            fragment.append(1, c);
        }
    }
    return true;
}

BOOL func_FileWildMatch(const char *path, const char *pat) {
    if (*pat != '\0') {
        if (*pat == '*') {
            if (func_FileWildMatch(path, pat + 1)) {
                return true;
            }
            if (*path == '\0') {
                return false;
            }
        } else {
            if (*pat == '?') {
                return *path != '\0' && func_FileWildMatch(path + 1, pat + 1);
            }
            if (tolower(*pat) != tolower(*path)) {
                return false;
            }
            ++pat;
        }
        return func_FileWildMatch(path + 1, pat);
    }
    return *path == '\0';
}
