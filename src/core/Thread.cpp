#include "Thread.h"
#include <Windows.h>

unsigned int dwThreadId; // 0x010A635C

// 0x00413AD0
unsigned int Moho::THREAD_GetMainThreadId() {
    return dwThreadId;
}

// 0x00413AA0
void Moho::THREAD_InitInvoke() {
    dwThreadId = GetCurrentThreadId();
}

// 0x00413C50
void Moho::THREAD_InvokeAsync(boost::function<void> fn, unsigned int id) {
    if (id == 0) {
        id = Moho::THREAD_GetMainThreadId();
    }
    HANDLE handle = OpenThread(THREAD_ALL_ACCESS, 0, id);
    if (handle != 0) {
        unsigned int *v9 = operator new(0x20);
        if (v9 != nullptr) {
            *v9 = 0;
            if (fn.manager) {
                *v9 = fn.manager;
                fn.manager(&fn.invoker_type, v9 + 2, 0);
            }
        }
        QueueUserAPC(pfnAPC, handle, v9);
        CloseHandle(handle);
    }
    if (fn.manager  && *fn.manager) {
        fn.manager(&fn.invoker_type, &fn.invoker_type, 1);
    }
}

// 0x00413C50
void Moho::THREAD_InvokeWait(boost::function<void> fn, unsigned int id) {
    if (id == 0) {
        id = Moho::THREAD_GetMainThreadId();
    }
    HANDLE handle = OpenThread(THREAD_ALL_ACCESS, 0, id);
    if (handle) {
        boost::barrier barr{2};
        unsigned int *v10 = operator new(8u);
        if (v10 != nullptr) {
            unsigned int *v11 = operator new(0x20u);
            if (v11 != nullptr) {
                *v11 = 0;
                if (fn.manager) {
                    *v11 = fn.manager;
                    fn.manager(&fn.invoker_type, v11 + 2, 0);
                }
            }
            v10[0] = v11;
            v10[1] = &barr;
        }
        QueueUserAPC(pfnAPC, handle, v10);
        barr.wait()
        CloseHandle(handle);
    }
    if (fn.manager != nullptr && *fn.manager != nullptr) {
        fn.manager(&fn.invoker_type, &fn.invoker_type, 1);
    }
}

// 0x00413AB0
bool Moho::THREAD_IsMainThread() {
    return GetCurrentThreadId() == Moho::THREAD_GetMainThreadId();
}

// 0x004141A0
void Moho::THREAD_SetAffinity(bool a1) {
    ULONG_PTR proc, sys;
    GetProcessAffinityMask(GetCurrentProcess(), &proc, &sys);
    for (int k = 0, i = 31; k <= 32; ++k, --i) {
        unsigned int mask = 1 << (a1 ? k : i);
        if ((mask & proc) != 0) {   
            SetThreadAffinityMask(GetCurrentThread(), mask);
            return;
        }
    }
}
