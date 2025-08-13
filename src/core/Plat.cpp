#include "Plat.h"
#include <Windows.h>
#include "core/Win.h"

std::vector<std::wstring> sErrorReportFilenames; // 0x010A87A8
std::wstring sErrorReportOutputDir; // 0x00F59358

bool sPlatGuard; // 0x010A6382
bool sSymbolHandlerInit; // 0x010A6383
boost::mutex *sPMutexSymHandler; // 0x010A6388
boost::once_flag once_InitSymHandlerMutex = BOOST_ONCE_INIT; // 0x010A638C

// 0x004A0ED0
void Moho::PLAT_RegisterFileForErrorReport(const wchar_t *file) {
    if (file != nullptr && file[0] != '\0') {
        if (! sErrorReportFilenames.empty()) {
            for (auto itr = sErrorReportFilenames.begin(); itr != error_report_filenames.end(); ++itr) {
                if (itr->compare(file) == 0) {
                    return;
                }
            }
        }
        std::wstring filename{file};
        sErrorReportFilenames.push_back(filename);
    }
}

// 0x004A0FC0
void Moho::PLAT_InitErrorReportOutputDir(const wchar_t *dir) {
    sErrorReportOutputDir = dir;
    if (sErrorReportOutputDir.size() != 0 &&
        sErrorReportOutputDir[sErrorReportOutputDir.size() - 1] != '\\'
    ) {
        sErrorReportOutputDir += L"\\";
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
        std::string err = Moho::WIN_GetLastError();
        gpg::Warnf("PLAT_CreateGameLogForReport(\"%s\") log file creation failed: %s",
                filename, err.c_str());
        return;
    }
    int numWritten = 0;
    if (WriteFile(handle, recentLines.c_str(), recentLines.size(), &numWritten, 0) != 0) {
        Moho::PLAT_RegisterFileForErrorReport(filename);
        CloseHandle(handle);
    } else {
        std::string err = Moho::WIN_GetLastError();
        gpg::Warnf("PLAT_CreateGameLogForReport(\"%s\") log file writing failed: %s",
                filename, err.c_str());
        CloseHandle(handle);
    }
}

// 0x004A2150
void Moho::PLAT_Init() {
    if (sPMutexSymHandler == nullptr) {
        boost::call_once(func_InitSymHandlerMutex, &once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{sPMutexSymHandler};
    if (! sPlatGuard) {
        sSymbolHandlerInit = false;
        SymSetOptions(
            SYMOPT_FAIL_CRITICAL_ERRORS |
            SYMOPT_LOAD_LINES |
            SYMOPT_DEFERRED_LOADS |
            SYMOPT_UNDNAME
        );
        if (SymInitialize(GetCurrentProcess(), 0, 1)) {
            sSymbolHandlerInit = true;
        }
        CreateMutexA(0, 0, "GPG_MohoEngine_Mutex");
        sPlatGuard = true;
    }
}

// 0x004A2210
void Moho::PLAT_Exit() {
    if (sPMutexSymHandler == nullptr) {
        boost::call_once(func_InitSymHandlerMutex, &once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{sPMutexSymHandler};
    if (sPlatGuard) {
        if (sSymbolHandlerInit) {
            CurrentProcess = GetCurrentProcess();
            SymCleanup(CurrentProcess);
            sSymbolHandlerInit = false;
        }
        sPlatGuard = false;
    }
}

// 0x004A22B0
unsigned int Moho::PLAT_GetCallStack(void *ptr, unsigned int stacks, unsigned int *dat) {
    PCONTEXT ctxt = (PCONTEXT) ptr;
    if (sPMutexSymHandler == nullptr) {
        boost::call_once(func_InitSymHandlerMutex, &once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{sPMutexSymHandler};
    if (sSymbolHandlerInit) {
        STACKFRAME stackFrame;
        memset(&stackFrame, 0, sizeof(stackFrame));
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrStack.Mode = AddrModeFlat;
        int eip, esp, ebp;
        if (ctxt != nullptr) {
            eip = ctxt->Eip;
            esp = ctxt->Esp;
            ebp = ctxt->Ebp;
        } else {
            sub_4A1EB0(&savedregs, &dword_1103940, &dword_1103944, &dword_1103948);
            eip = dword_1103940;
            esp = dword_1103944;
            ebp = dword_1103948;
        }
        stackFrame.AddrFrame.Offset = ebp;
        stackFrame.AddrStack.Offset = esp;
        stackFrame.AddrPC.Offset = eip;
        int k = 0;
        while (k < stacks) {
            if (! StackWalk(
                IMAGE_FILE_MACHINE_I386,
                GetCurrentProcess(),
                GetCurrentThread(),
                &stackFrame,
                nullptr,
                nullptr,
                SymFunctionTableAccess,
                SymGetModuleBase,
                nullptr
            )) {
                break;
            }
            if (stackFrame.AddrPC.Offset != 0) {
                if (k != 0) {
                    stackFrame.AddrPC.Offset -= 5;
                }
                dat[k++] = stackFrame.AddrPC.Offset;
            }
        }
        return k;
    }
    return 0;
}

// 0x004A2440
bool Moho::PLAT_GetSymbolInfo(unsigned int addr, Moho::SPlatSymbolInfo &info) {
    if (sPMutexSymHandler == nullptr) {
        boost::call_once(func_InitSymHandlerMutex, &once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{sPMutexSymHandler};
    if (sSymbolHandlerInit) {
        IMAGEHLP_SYMBOL symbol;
        symbol.SizeOfStruct = 24;
        symbol.MaxNameLength = 233;
        DWORD symDisplacement;
        if (SymGetSymFromAddr(GetCurrentProcess(), addr, &symDisplacement, &symbol)) {
            info.mAddr = addr;
            info.mSymbol = symbol.Name;
            info.mSymDis = symDisplacement;
            IMAGEHLP_LINE line;
            line.SizeOfStruct = 20;
            DWORD lineDisplacement;
            if (SymGetLineFromAddr(GetCurrentProcess(), addr, &lineDisplacement, &line)) {
                info.mFilename = line.FileName;
                info.mLineNum = line.LineNumber;
                info.mLineDis = lineDisplacement;
            } else {
                info.mFilename = "(Unknown)";
                info.mLineNum = 0;
                info.mLineDis = 0;
            }
            return true;
        }
    }
    return false;
}

// 0x004A25D0
std::string Moho::PLAT_UnDecorateSymbolName(const char *name, bool nameOnly) {
    if (sPMutexSymHandler == nullptr) {
        boost::call_once(func_InitSymHandlerMutex, &once_InitSymHandlerMutex);
    }
    boost::mutex::scoped_lock lock{sPMutexSymHandler};
    if (name != nullptr) {
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
        if (Moho::PLAT_GetSymbolInfo(addrs[start], info)) {
            builder.append(gpg::STR_Printf(
                "\t%s + %d bytes (%s(%d) + %d bytes)\r\n",
                info.mSymbol.c_str(),
                info.mSymDis,
                info.mFilename.c_str(),
                info.mLineNum,
                info.mLineDis
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
unsigned int Moho::PLAT_GetRegistryValue(const char *key, LPBYTE dest) {
    std::vector<char> buf{};
    if (key == nullptr || dest == nullptr) {
        return 0;
    }
    buf.resize(strlen(key) + 1, '\0');
    char *reg = strcpy(buf.data(), key);
    char *v3 = strchr(reg, '\\');
    const char *openSubKey = v3;
    char *v5, *readSubKey;
    if (v3 != nullptr &&
        (openSubKey = v3 + 1, *v3 = '\0', v3 != (char *)-1) &&
        (v5 = strrchr(openSubKey, '\\')) != 0
    ) {
        *v5 = 0;
        readSubKey = v5 + 1;
    } else {
        readSubKey = openSubKey;
        openSubKey = nullptr;
    }
    HKEY regKey;
    if (stricmp(reg, "HKEY_CLASSES_ROOT") == 0) {
        regKey = HKEY_CLASSES_ROOT;
    } else if (stricmp(reg, "HKEY_CURRENT_USER") == 0) {
        regKey = HKEY_CURRENT_USER;
    } else if (stricmp(reg, "HKEY_LOCAL_MACHINE") == 0) {
        regKey = HKEY_LOCAL_MACHINE;
    } else if (stricmp(reg, "HKEY_USERS") == 0) {
        regKey = HKEY_USERS;
    } else if (stricmp(reg, "HKEY_CURRENT_CONFIG") == 0) {
        regKey = HKEY_CURRENT_CONFIG;
    } else if (stricmp(reg, "HKEY_DYN_DATA") == 0) {
        regKey = HKEY_DYN_DATA;
    } else if (stricmp(reg, "HKEY_PERFORMANCE_DATA") == 0) {
        regKey = HKEY_PERFORMANCE_DATA;
    } else {
        regKey = HKEY_CURRENT_USER;
    }
    HKEY handle;
    if (RegOpenKeyExA(regKey, openSubKey, 0, KEY_EXECUTE, &handle)) {
        gpg::Logf("PLAT_GetRegistryValue: Unable to open registry key \"%s\"", a1);
        return 0;
    }
    DWORD size = 256;
    if (RegQueryValueExA(handle, readSubKey, 0, nullptr, dest, &size)) {
        RegCloseKey(handle);
        gpg::Logf("PLAT_GetRegistryValue: Unable to read registry key \"%s\"", a1);
        return 0;
    }
    RegCloseKey(handle);
    return size;
}

// 0x004A2F60
bool Moho::PLAT_SetRegistryValueDword(const char *key, unsigned int val) {
    return Moho::PLAT_SetRegistryValue(key, (BYTE *)&val, sizeof(val), RRF_RT_REG_EXPAND_SZ);
}

// 0x004A2F80
bool Moho::PLAT_SetRegistryValueString(const char *key, char *val) {
    if (KEY_ALL_ACCESS) {
        return Moho::PLAT_SetRegistryValue(key, val, strlen(val) + 1, RRF_RT_REG_NONE);
    } else {
        return Moho::PLAT_SetRegistryValue(key, (BYTE *)"", 1, RRF_RT_REG_NONE);
    }
}

// 0x004A1F10
bool Moho::PLAT_SetRegistryValue(const char *key, char *data, DWORD size, DWORD dwType) {
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
    if (key == nullptr) {
        return 0;
    }
    buf.resize(strlen(key) + 1, 0);
    reg = strcpy(buf.data(), key);
    v6 = strchr(v5, '\\');
    v7 = v6;
    if (v6 &&
        (v7 = v6 + 1, *v6 = 0, v6 != (char *)-1) &&
        (v8 = strrchr(v7, '\\')) != 0) {
        *v8 = 0;
        v9 = v8 + 1;
    } else {
        v9 = v7;
        v7 = 0;
    }
    HKEY regKey;
    if (stricmp(reg, "HKEY_CLASSES_ROOT") == 0) {
        regKey = HKEY_CLASSES_ROOT;
    } else if (stricmp(reg, "HKEY_CURRENT_USER") == 0) {
        regKey = HKEY_CURRENT_USER;
    } else if (stricmp(reg, "HKEY_LOCAL_MACHINE") == 0) {
        regKey = HKEY_LOCAL_MACHINE;
    } else if (stricmp(reg, "HKEY_USERS") == 0) {
        regKey = HKEY_USERS;
    } else if (stricmp(reg, "HKEY_CURRENT_CONFIG") == 0) {
        regKey = HKEY_CURRENT_CONFIG;
    } else if (stricmp(reg, "HKEY_DYN_DATA") == 0) {
        regKey = HKEY_DYN_DATA;
    } else if (stricmp(reg, "HKEY_PERFORMANCE_DATA") == 0) {
        regKey = HKEY_PERFORMANCE_DATA;
    } else {
        regKey = HKEY_CURRENT_USER;
    }
    HKEY handle;
    if (RegCreateKeyExA(regKey, v7, 0, 0, 0, 0xF003Fu, 0, &handle, 0)) {
        gpg::Logf("PLAT_SetRegistryValue: Unable to create registry key \"%s\"", key);
        return false;
    } else if (RegSetValueExA(handle, v9, 0, type, data, size)) {
        RegCloseKey(handle);
        gpg::Logf("PLAT_SetRegistryValue: Unable to write registry key \"%s\"", key);
        return false;
    } else {
        RegCloseKey(handle);
        return true;
    }
}

// 0x004A2B30
LONG TopLevelExceptionFilter(_EXCEPTION_POINTERS *exceptionInfo) {
    if (! Moho::CFG_GetArgOption("/bugreport", 0, nullptr) && Moho::CFG_GetArgOption("/nobugreport", 0, nullptr)) {
        unsigned int exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
        if (exceptionCode != 0x80000003) {
            std::string msg = gpg::STR_Printf(
                "%s (0x%08x) at address 0x%08x",
                func_StructuredExceptionToString(exceptionCode),
                exceptionCode,
                exceptionInfo->ContextRecord->Eip
            );
            if (exceptionCode == 0xC0000005) {
                ULONG_PTR &info = exceptionInfo->ExceptionRecord->ExceptionInformation;
                msg.append(gpg::STR_Printf(
                    "\n    attempted to %s memory at 0x%08x",
                    info[0] ? "write" : "read",
                    info[1]
                ));
            }
            Moho::WIN_ShowCrashDialog("Unhandled Exception", msg.c_str(), exceptionInfo, 0);
        }
        return false;
    } else {
        DestroyWindow((HWND)sMainWindow->m_hWnd);
        HANDLE toolhelp32Snapshot = CreateToolhelp32Snapshot(4, 0);
        THREADENTRY32 te;
        te.dwSize = 28;
        Thread32First(toolhelp32Snapshot, &te);
        do {
            if (te.th32OwnerProcessID == GetCurrentProcessId() && te.th32ThreadID != GetCurrentThreadId()) {
                SuspendThread(OpenThread(2, 0, te.th32ThreadID));
            }
        } while (Thread32Next(toolhelp32Snapshot, &te));
        ReportFault(exceptionInfo, 0);
        sBugSplatMiniDmpSender.setCallback(sub_4A1610);
        sBugSplatMiniDmpSender.createReport(exceptionInfo);
        return true;
    }
}

// 0x004A2930
const char *func_StructuedExceptionToString(unsigned int err) {
    switch (err) {
        case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
        case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
        case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
        case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
        case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
        case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
        case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
        case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
        case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
        case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
        case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
        case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
        case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
        case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
        case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
        default: return "Unknown structured exception";
    }
}

// 0x004A1E20
void func_InitSymHandlerMutex() {
    static boost::mutex sMutexSymHandler; // 0x010C7958
    sPMutexSymHandler = &sMutexSymHandler;
}
