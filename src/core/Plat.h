#include <string>
#include <vector>
#include "boost/thread/mutex.hpp"

static std::vector<std::wstring> error_report_filenames; // 0x010A87A8
static std::wstring error_report_output_dir; // 0x00F59358

static bool plat_guard; // 0x010A6382
static bool symbol_handler_init; // 0x010A6383
static boost::mutex *mutex_sym_handlerp; // 0x010A6388
static int call_once_InitSymHandlerMutex; // 0x010A638C

static MiniDmpSender bugsplat_sender; // 0x010A87B8

namespace Moho {

struct SPlatSymbolInfo
{
    size_t addr;
    std::string symbol;
    size_t symDis;
    std::string filename;
    size_t lineNum;
    size_t lineDis;
};


void PLAT_RegisterFileForErrorReport(const wchar_t *); // 0x004A0ED0
void PLAT_InitErrorReportOutputDir(const wchar_t *); // 0x004A0FC0
void PLAT_CreateGameLogForReport(); // 0x004A1230
void PLAT_Init(); // 0x004A2150
void PLAT_Exit(); // 0x004A2210
unsigned int PLAT_GetCallStack(void *, unsigned int, unsigned int *); // 0x004A22B0
bool PLAT_GetSymbolInfo(unsigned int, Moho::SPlatSymbolInfo &); // 0x004A2440
std::string PLAT_UnDecorateSymbolName(const char *, bool); // 0x004A25D0
std::string Moho::PLAT_FormatCallstack(int, int, const unsigned int *); // 0x004A26E0
void PLAT_CatchStructuredExceptions(); // 0x004A2D30
unsigned int PLAT_GetRegistryValue(const char , LPBYTE); // 0x004A2D40
bool PLAT_SetRegistryValueDword(const char *, unsigned int); // 0x004A2F60
bool PLAT_SetRegistryValueString(const char *, const char *); // 0x004A2F80
bool PLAT_SetRegistryValue(const char *, char *, DWORD, DWORD); // 0x004A1F10

}

LONG TopLevelExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo); // 0x004A2B30
const char *func_StructuedExceptionToString(unsigned int err); // 0x004A2930
void func_InitSymHandlerMutex(); // 0x004A1E20

