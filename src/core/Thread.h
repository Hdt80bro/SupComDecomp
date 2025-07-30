#include "boost/function.hpp"

static unsigned int dwThreadId; // 0x010A635C

namespace Moho {

unsigned int THREAD_GetMainThreadId(); // 0x00413AD0
void THREAD_InitInvoke(); // 0x00413AA0
void THREAD_InvokeAsync(boost::function<void>, unsigned int); // 0x00413C50
void THREAD_InvokeWait(boost::function<void>, unsigned int); // 0x00413C50
bool THREAD_IsMainThread(); // 0x00413AB0
void THREAD_SetAffinity(bool); // 0x004141A0

}
