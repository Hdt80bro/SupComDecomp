// known file

#include "ScrUnsafe.h"
#include "Windows.h"


// 0x00F596C4
Moho::CScrLuaInitFormSet Moho::scr_UnsafeInits{"Unsafe"};

// 0x00F61170
std::pair<const char *, int> sUnsafePaths[] = {
    {"DESKTOP", 0},
    {"PERSONAL", 5},
    {"CSIDL_FAVORITES", 6},
    {"CSIDL_STARTUP", 7},
    {"CSIDL_RECENT", 8},
    {"SENDTO", 9},
    {"BITBUCKET", 0xA},
    {"STARTMENU", 0xB},
    {"MYDOCUMENTS", 0xC},
    {"MYMUSIC", 0xD},
    {"MYVIDEO", 0xE},
    {"DESKTOPDIRECTORY", 0x10},
    {"FONTS", 0x14},
    {"TEMPLATES", 0x15},
    {"COMMON_STARTMENU", 0x16},
    {"COMMON_PROGRAMS", 0x17},
    {"COMMON_STARTUP", 0x18},
    {"COMMON_DESKTOPDIRECTORY", 0x19},
    {"APPDATA", 0x1A},
    {"LOCAL_APPDATA", 0x1C},
    {"COMMON_FAVORITES", 0x1F},
    {"COMMON_APPDATA", 0x23},
    {"PROGRAM_FILES", 0x26},
    {"MYPICTURES", 0x27},
    {"PROFILE", 0x28},
    {"SYSTEMX86", 0x29},
    {"PROGRAM_FILESX86", 0x2A},
    {"PROGRAM_FILES_COMMON", 0x2B},
    {"PROGRAM_FILES_COMMONX86", 0x2C},
    {"COMMON_TEMPLATES", 0x2D},
    {"COMMON_DOCUMENTS", 0x2E},
    {"COMMON_MUSIC", 0x35},
    {"COMMON_PICTURES", 0x36},
    {"COMMON_VIDEO", 0x37},
    {nullptr, 0}
};

namespace Moho {

namespace {
// 0x010A9230 : init form
// 0x004D6940 : cfunc
MOHO_LUA_FUNC(Moho::scr_UnsafeInits, SHGetFolderPath, "(name, create?) -- Interface to Win32 SHGetFolderPath api") {
    MOHO_LUA_EXPECT_ARGS(1);
    LuaPlus::LuaStack stack{state};
    const char *arg1 = stack[1].GetString();
    if (arg1 == nullptr) {
        stack[1].TypeError("string");
    }
    for (auto itr = &sUnsafePaths[0]; itr->first != nullptr; ++itr) {
        if (strcmp(arg1, itr->first) == 0) {
            static wchar_t sPathBuff[260]; // 0x01103CD8
            int result = ::SHGetFolderPathW(0, itr->second, 0, 0, &sPathBuff);
            if (result != 0) {
                state->Error("SHGetFolderPath failed: %x", result);
            }
            std::string path = gpg::STR_WideToUtf8((const wchar_t *) &sPathBuff);
            if (path.c_str()[path.size()] != '\\') {
                path.append("\\", 1);
            }
            state->PushString(path.c_str());
            return 1;
        }
    }
    state->Error("Unknown id for SHGetFolderPath: %s", arg1);
    GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 95, "c:\\work\\rts\\main\\code\\src\\core\\ScrUnsafe.cpp");
}
}

}
