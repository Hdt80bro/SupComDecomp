#include "Thread.h"
#include <Windows.h>

unsigned int sMainThreadId; // 0x010A635C

// 0x00413AD0
unsigned int Moho::THREAD_GetMainThreadId() {
    return sMainThreadId;
}

// 0x00413AA0
void Moho::THREAD_InitInvoke() {
    sMainThreadId = GetCurrentThreadId();
}

// 0x00413C50
void Moho::THREAD_InvokeAsync(boost::function<void> fn, unsigned int id) {
    if (id == 0) {
        id = Moho::THREAD_GetMainThreadId();
    }
    HANDLE handle = OpenThread(THREAD_ALL_ACCESS, 0, id);
    if (handle != 0) {
        boost::function<void> *f = new boost::function<void>{fn};
        QueueUserAPC(&Moho::THREAD_Call, handle, f);
        CloseHandle(handle);
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
        unsigned int *v10 = new unsigned int[2];
        if (v10 != nullptr) {
            v10[0] = new boost::function<void>{fn};
            v10[1] = &barr;
        }
        QueueUserAPC(&Moho::THREAD_Call, handle, v10);
        barr.wait()
        CloseHandle(handle);
    }
}

// 0x00413AB0
bool Moho::THREAD_IsMainThread() {
    return GetCurrentThreadId() == Moho::THREAD_GetMainThreadId();
}

// 0x00413AE0
void THREAD_Call/*guess*/(boost::function<void> *fn) {
    (*fn)();
    delete(fn);
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
