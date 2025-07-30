#include "Plat.h"
#include <Windows.h>
#include "core/Win.h"

std::vector<std::wstring> error_report_filenames; // 0x010A87A8
std::wstring error_report_output_dir; // 0x00F59358

bool plat_guard; // 0x010A6382
bool symbol_handler_init; // 0x010A6383
boost::mutex *mutex_sym_handlerp; // 0x010A6388
int call_once_InitSymHandlerMutex; // 0x010A638C

// 0x004A0ED0
void Moho::PLAT_RegisterFileForErrorReport(const wchar_t *file) {
    if (file != nullptr && file[0] != '\0') {
        if (! error_report_filenames.empty()) {
            for (auto itr = error_report_filenames.begin(); itr != error_report_filenames.end(); ++itr) {
                if (itr->compare(file) == 0) {
                    return;
                }
            }
        }
        std::wstring filename{file, wcslen(file)};
        error_report_filenames.push_back(filename);
    }
}

// 0x004A0FC0
void Moho::PLAT_InitErrorReportOutputDir(const wchar_t *dir) {
    error_report_output_dir = dir;
    if (error_report_output_dir.size() != 0 &&
        error_report_output_dir[error_report_output_dir.size() - 1] != '\\'
    ) {
        error_report_output_dir += L"\\";
    }
}

// 0x004A1230
void Moho::PLAT_CreateGameLogForReport() {
    std::string recentLines = Moho::LOG_GetRecentLines();
    std::wstring title = gpg::STR_Utf8ToWide(supcomapp->title.c_str());
    std::wstring strFilename = error_report_output_dir + title + L".sclog";
    const wchar_t *filename = strFilename.c_str();
    int handle = CreateFileW(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_READ_ATTRIBUTES, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        gpg::Warnf("PLAT_CreateGameLogForReport(\"%s\") log file creation failed: %s", 
            filename, Moho::WIN_GetLastError().c_str());
        return;
    }
    if (WriteFile(handle, recentLines.c_str(), recentLines.size(), &NumberOfBytesWritten, 0) != 0) {
        Moho::PLAT_RegisterFileForErrorReport(filename);
        CloseHandle(handle);
    } else {
        gpg::Warnf("PLAT_CreateGameLogForReport(\"%s\") log file writing failed: %s",
            filename, Moho::WIN_GetLastError().c_str());
        CloseHandle(handle);
    }
}

// 0x004A2150
void Moho::PLAT_Init() {
    if (mutex_sym_handlerp == nullptr) {
        call_once(func_InitSymHandlerMutex, &call_once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{mutex_sym_handlerp};
    if (! plat_guard) {
        symbol_handler_init = false;
        SymSetOptions(
            SYMOPT_FAIL_CRITICAL_ERRORS |
            SYMOPT_LOAD_LINES |
            SYMOPT_DEFERRED_LOADS |
            SYMOPT_UNDNAME
        );
        if (SymInitialize(GetCurrentProcess(), 0, 1)) {
            symbol_handler_init = true;
        }
        CreateMutexA(0, 0, "GPG_MohoEngine_Mutex");
        plat_guard = true;
    }
}

// 0x004A2210
void Moho::PLAT_Exit() {
    if (mutex_sym_handlerp == nullptr) {
        call_once(func_InitSymHandlerMutex, &call_once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{mutex_sym_handlerp};
    if (plat_guard) {
        if (symbol_handler_init) {
            CurrentProcess = GetCurrentProcess();
            SymCleanup(CurrentProcess);
            symbol_handler_init = false;
        }
        plat_guard = false;
    }
}

// 0x004A22B0
unsigned int Moho::PLAT_GetCallStack(void *ptr, unsigned int stacks, unsigned int *dat) {
    PCONTEXT ctxt = (PCONTEXT) ptr;
    if (mutex_sym_handlerp == nullptr) {
        call_once(func_InitSymHandlerMutex, &call_once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{mutex_sym_handlerp};
    if (symbol_handler_init) {
        _tagSTACKFRAME StackFrame;
        memset(&StackFrame, 0, sizeof(StackFrame));
        StackFrame.AddrFrame.Mode = AddrModeFlat;
        StackFrame.AddrPC.Mode = AddrModeFlat;
        StackFrame.AddrStack.Mode = AddrModeFlat;
        int Eip, Esp, Ebp;
        if (ctxt != nullptr) {
            Eip = ctxt->Eip;
            Esp = ctxt->Esp;
            Ebp = ctxt->Ebp;
        } else {
            sub_4A1EB0((int)&savedregs, &dword_1103940, &dword_1103944);
            Eip = dword_1103940;
            Esp = dword_1103944;
            Ebp = dword_1103948;
        }
        StackFrame.AddrFrame.Offset = Ebp;
        StackFrame.AddrStack.Offset = Esp;
        StackFrame.AddrPC.Offset = Eip;
        int k = 0
        while (k < stacks) {
            if (! StackWalk(
                IMAGE_FILE_MACHINE_I386,
                GetCurrentProcess(),
                GetCurrentThread(),
                &StackFrame,
                0,
                0,
                SymFunctionTableAccess,
                SymGetModuleBase,
                0
            )) {
                break;
            }
            if (StackFrame.AddrPC.Offset != 0) {
                if (k) {
                    StackFrame.AddrPC.Offset -= 5;
                }
                dat[k++] = StackFrame.AddrPC.Offset;
            }
        }
        return k;
    }
    return 0;
}

// 0x004A2440
bool Moho::PLAT_GetSymbolInfo(unsigned int addr, Moho::SPlatSymbolInfo &info) {
    if (mutex_sym_handlerp == nullptr) {
        call_once(func_InitSymHandlerMutex, &call_once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{mutex_sym_handlerp};
    if (symbol_handler_init) {
        _IMAGEHLP_SYMBOL Symbol;
        Symbol.SizeOfStruct = 24;
        Symbol.MaxNameLength = 233;
        DWORD symDisplacement;
        if (SymGetSymFromAddr(GetCurrentProcess(), addr, &symDisplacement, &Symbol)) {
            info.addr = addr;
            info.symbol = Symbol.Name;
            info.symDis = symDisplacement;
            _IMAGEHLP_LINE Line;
            Line.SizeOfStruct = 20;
            DWORD lineDisplacement;
            if (SymGetLineFromAddr(GetCurrentProcess(), addr, &lineDisplacement, &Line)) {
                info.filename = Line.FileName;
                info.lineNum = Line.LineNumber;
                info.lineDis = lineDisplacement;
            } else {
                info.filename = "(Unknown)";
                info.lineNum = 0;
                info.lineDis = 0;
            }
            return true;
        }
    }
    return false;
}

// 0x004A25D0
std::string Moho::PLAT_UnDecorateSymbolName(const char *name, bool nameOnly) {
    if (mutex_sym_handlerp == nullptr) {
        call_once(func_InitSymHandlerMutex, &call_once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{mutex_sym_handlerp};
    if (name) {
        char outputString[1024];
        if (UnDecorateSymbolName(name, outputString, sizeof(outputString) - 1, nameOnly ? UNDNAME_NAME_ONLY : UNDNAME_COMPLETE)) {
            return std::string{outputString};
        } else {
            return std::string{name};
        }
    } else {
        return std::string{};
    }
}

// 0x004A26E0
std::string Moho::PLAT_FormatCallstack(int start, int end, const unsigned int *addrs) {
    std::string builder{};
    for ( ; start < end; ++start) {
        Moho::SPlatSymbolInfo info{};
        if (Moho::PLAT_GetSymbolInfo(addrs[start], &info)) {
            builder.append(gpg::STR_Printf(
                "\t%s + %d bytes (%s(%d) + %d bytes)\r\n",
                info.symbol.c_str(),
                info.symDis,
                info.filename.c_str(),
                info.lineNum,
                info.lineDis
            ));
        } else {
            builder.append(gpg::STR_Printf(
                "\tUnknown symbol (address 0x%08x)\r\n", addrs[start]
            ));
        }
    }
    return builder;
}

// 0x004A2D30
void Moho::PLAT_CatchStructuredExceptions() {
    return SetUnhandledExceptionFilter(TopLevelExceptionFilter);
}

// 0x004A2D40
unsigned int Moho::PLAT_GetRegistryValue(const char *a1, LPBYTE lpData) {
    const char *v2; // ebp
    char *v3; // eax
    const char *v4; // esi
    char *v5; // eax
    const CHAR *v6; // edi
    int v7; // eax
    DWORD v9; // esi
    HKEY phkResult; // [esp+10h] [ebp-24h] BYREF
    DWORD cbData; // [esp+14h] [ebp-20h] BYREF
    std::vector v12; // [esp+18h] [ebp-1Ch] BYREF

    std::vector<char> buf;
    if (a1 == nullptr || lpData == nullptr) {
        return 0;
    }
    buf.resize(strlen(a1) + 1, 0);
    const char *key = strcpy(buf.data(), a1);
    char *v3 = strchr(key, '\\');
    const char *v4 = v3;
    if (v3 && (v4 = v3 + 1, *v3 = 0, v3 != (char *)-1) && (v5 = strrchr(v4, 92)) != 0) {
        *v5 = 0;
        v6 = v5 + 1;
    } else {
        v6 = v4;
        v4 = 0;
    }
    int reg;
    if (stricmp(key, "HKEY_CLASSES_ROOT") == 0) {
        reg = 0x80000000;
    } else if (stricmp(key, "HKEY_CURRENT_USER") == 0) {
        reg = 0x800000017;
    } else if (stricmp(key, "HKEY_LOCAL_MACHINE") == 0) {
        reg = 0x80000002;
    } else if ( !stricmp(key, "HKEY_USERS") == 0) {
        reg = 0x80000003;
    } else if (stricmp(key, "HKEY_CURRENT_CONFIG")  == 0) {
        reg = 0x80000005;
    } else if (stricmp(key, "HKEY_DYN_DATA") == 0) {
        reg = 0x80000006;
    } else if (stricmp(key, "HKEY_PERFORMANCE_DATA") != 0) {
        reg = 0x80000001;
    } else {
        reg = 0x80000004;
    }
    HKEY phkResult;
    if (RegOpenKeyExA((HKEY) reg, v4, 0, 0x20019u, &phkResult)) {
        gpg::Logf("PLAT_GetRegistryValue: Unable to open registry key \"%s\"", a1);
        return 0;
    }
    DWORD cbData = 256;
    if (RegQueryValueExA(phkResult, v6, 0, 0, lpData, &cbData)) {
        RegCloseKey(phkResult);
        gpg::Logf("PLAT_GetRegistryValue: Unable to read registry key \"%s\"", a1);
        return 0;
    }
    RegCloseKey(phkResult);
    return cbData;
}

// 0x004A2F60
bool Moho::PLAT_SetRegistryValueDword(const char *ket, unsigned int val) {
    return Moho::PLAT_SetRegistryValue(key, (BYTE *)&val, sizeof(val), RRF_RT_REG_EXPAND_SZ);
}

// 0x004A2F80
bool Moho::PLAT_SetRegistryValueString(const char *ket, char *val) {
    if (a1) {
        return Moho::PLAT_SetRegistryValue(key, val, strlen(val) + 1, RRF_RT_REG_NONE);
    } else {
        return Moho::PLAT_SetRegistryValue(key, (BYTE *)"", 1u, RRF_RT_REG_NONE);
    }
}

// 0x004A1F10
bool Moho::PLAT_SetRegistryValue(const char *a1, char *lpData, DWORD cbData, DWORD dwType) {
    char *v5; // ebp
    char *v6; // eax
    const char *v7; // esi
    char *v8; // eax
    const CHAR *v9; // edi
    int v10; // eax
    HKEY phkResult; // [esp+8h] [ebp-20h] BYREF
    std::vector v12; // [esp+Ch] [ebp-1Ch] BYREF
    int v13; // [esp+24h] [ebp-4h]

    
    std::vector<char> buf;
    if (a1 == nullptr) {
        return 0;
    }
    buf.resize(strlen(a1) + 1, 0);
    v5 = strcpy(buf.data(), a1);
    v6 = strchr(v5, '\\');
    v7 = v6;
    if (v6 && (v7 = v6 + 1, *v6 = 0, v6 != (char *)-1) && (v8 = strrchr(v7, 92)) != 0) {
        *v8 = 0;
        v9 = v8 + 1;
    } else {
        v9 = v7;
        v7 = 0;
    }
    if (stricmp(v5, "HKEY_CLASSES_ROOT") == 0) {
        v10 = 0x80000000;
    } else if (stricmp(v5, "HKEY_CURRENT_USER") != 0) {
        v10 = 0x80000001;
    } else if ( !stricmp(v5, "HKEY_LOCAL_MACHINE") != 0) {
        v10 = 0x80000002;
    } else if (stricmp(v5, "HKEY_USERS") != 0) {
        v10 = 0x80000003;
    } else if (stricmp(v5, "HKEY_CURRENT_CONFIG") != 0) {
        v10 = 0x80000004;
    } else if (stricmp(v5, "HKEY_DYN_DATA") != 0) {
        v10 = 0x80000005;
    } else if (stricmp(v5, "HKEY_PERFORMANCE_DATA") != 0) {
        v10 = 0x80000001;
    } else {
        v10 = 0x80000004;
    }
    if (RegCreateKeyExA((HKEY)v10, v7, 0, 0, 0, 0xF003Fu, 0, &phkResult, 0)) {
        gpg::Logf("PLAT_SetRegistryValue: Unable to create registry key \"%s\"", a1);
        if (v5) {
            operator delete(v5);
        }
        return false;
    } else if (RegSetValueExA(phkResult, v9, 0, dwType, lpData, cbData)) {
        RegCloseKey(phkResult);
        gpg::Logf("PLAT_SetRegistryValue: Unable to write registry key \"%s\"", a1);
        return false;
    } else {
        RegCloseKey(phkResult);
        return true;
    }
}

// 0x004A2B30
LONG TopLevelExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo) {
    if (! Moho::CFG_GetArgOption("/bugreport", 0, nullptr) && Moho::CFG_GetArgOption("/nobugreport", 0, nullptr)) {
        unsigned int ExceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
        if (ExceptionCode != 0x80000003) {
            std::string msg = gpg::STR_Printf(
                "%s (0x%08x) at address 0x%08x",
                func_StructuredExceptionToString(ExceptionCode),
                ExceptionCode,
                ExceptionInfo->ContextRecord->Eip
            );
            if (ExceptionCode == 0xC0000005) {
                ULONG_PTR &info = ExceptionInfo->ExceptionRecord->ExceptionInformation;
                msg.append(gpg::STR_Printf(
                    "\n    attempted to %s memory at 0x%08x",
                    info[0] ? "write" : "read",
                    info[1]
                ));
            }
            Moho::WIN_ShowCrashDialog(0, ExceptionInfo, (HMODULE)"Unhandled Exception", msg.c_str*());
        }
        return false;
    } else {
        DestroyWindow((HWND)mainWindow->m_hWnd);
        HANDLE Toolhelp32Snapshot = CreateToolhelp32Snapshot(4u, 0);
        THREADENTRY32 te;
        te.dwSize = 28;
        Thread32First(Toolhelp32Snapshot, &te);
        do {
            if (te.th32OwnerProcessID == GetCurrentProcessId() && te.th32ThreadID != GetCurrentThreadId()) {
                SuspendThread(OpenThread(2u, 0, te.th32ThreadID));
            }
        } while (Thread32Next(Toolhelp32Snapshot, &te));
        ReportFault(ExceptionInfo, 0);
        bugsplat_miniDmpSender.setCallback(sub_4A1610);
        bugsplat_miniDmpSender.createReport(ExceptionInfo);
        return true;
    }
}

// 0x004A2930
const char *func_StructuedExceptionToString(unsigned int err) {
    switch (err) {
        case 0x80000002: return "EXCEPTION_DATATYPE_MISALIGNMENT";
        case 0x80000003: return "EXCEPTION_BREAKPOINT";
        case 0x80000004: return "EXCEPTION_SINGLE_STEP";
        case 0xC0000005: return "EXCEPTION_ACCESS_VIOLATION";
        case 0xC0000006: return "EXCEPTION_IN_PAGE_ERROR";
        case 0xC000001D: return "EXCEPTION_ILLEGAL_INSTRUCTION";
        case 0xC0000025: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
        case 0xC0000026: return "EXCEPTION_INVALID_DISPOSITION";
        case 0xC000008C: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
        case 0xC000008D: return "EXCEPTION_FLT_DENORMAL_OPERAND";
        case 0xC000008E: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
        case 0xC000008F: return "EXCEPTION_FLT_INEXACT_RESULT";
        case 0xC0000090: return "EXCEPTION_FLT_INVALID_OPERATION";
        case 0xC0000091: return "EXCEPTION_FLT_OVERFLOW";
        case 0xC0000092: return "EXCEPTION_FLT_STACK_CHECK";
        case 0xC0000093: return "EXCEPTION_FLT_UNDERFLOW";
        case 0xC0000094: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
        case 0xC0000095: return "EXCEPTION_INT_OVERFLOW";
        case 0xC0000096: return "EXCEPTION_PRIV_INSTRUCTION";
        case 0xC00000FD: return "EXCEPTION_STACK_OVERFLOW";
        default: return "Unknown structured exception";
    }
}

// 0x004A1E20
void func_InitSymHandlerMutex() {
    static boost::mutex mutex_sym_handler; // 0x010C7958
    mutex_sym_handlerp = &mutex_sym_handler;
}
