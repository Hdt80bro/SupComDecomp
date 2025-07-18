#include "Timer.h"

LARGE_INTEGER PerformanceFrequency; ; // 0x00F8ED58
float TimerCycleToSeconds; // 0x00F8ED60
LONGLONG gpg_time; // 0x00F8ED68
gpg::time::Timer system_timer; // 0x00F8ED78, first set at 0x00BEAB90

// inline
inline void initPerformanceCounters() {
    if (! PerformanceFrequency.QuadPart) {
        QueryPerformanceCounter(&PerformanceFrequency);
        TimerCycleToSeconds = 1.0 / PerformanceFrequency.QuadPart;
    }
}

// 0x009556F0 & 0x009556D0
gpg::time::Timer::Timer() :
    time{gpg::time::GetTime()}
{}

// 0x00955710
LONGLONG gpg::time::Timer::ElapsedCyclesAndReset() {
    LONGLONG curTime = gpg::time::GetTime();
    LONGLONG diff = curTime - this->time;
    this->time = curTime;
    return diff;
}

// 0x00955700
LONGLONG gpg::time::Timer::ElapsedCycles() {
    return gpg::time::GetTime() - this->time;
}

// 0x00485A40
float gpg::time::Timer::ElapsedMicroseconds() {
    return gpg::time::CyclesToMicroseconds(this->ElapsedCycles());
}

// 0x004A3560
float gpg::time::Timer::ElapsedSeconds() {
    return gpg::time::CyclesToSeconds(this->ElapsedCycles());
}

// 0x00955400
LONGLONG gpg::time::GetTime() {
    LARGE_INTEGER PerformanceCount;
    QueryPerformanceCounter(&PerformanceCount);
    LONGLONG newVal = PerformanceCount.QuadPart;
    LONGLONG ex, cur;
    do {
        cur = gpg_time;
        if (newVal < cur) {
            newVal = gpg_time + 1;
        }
        ex = InterlockedCompareExchange64(&gpg_time, newVal, gpg_time);
    } while (ex != cur);
    return newVal;
}

// 0x00955520
LONGLONG gpg::time::CyclesToMicroseconds(LONGLONG cycles) {
    initPerformanceCounters();
    LONGLONG freq = PerformanceFrequency.QuadPart;
    return 1000000 * (cycles % freq + (cycles >> 32) / freq);
}

// 0x009554E0
float gpg::time::CyclesToMilliseconds(LONGLONG cycles) {
    initPerformanceCounters();
    return cycles * TimerCycleToSeconds * 1000.0;
}

// 0x009554A0
float gpg::time::CyclesToSeconds(LONGLONG cycles) {
    initPerformanceCounters();
    return cycles * TimerCycleToSeconds;
}

// 0x00955730
gpg::time::Timer const &gpg::time::GetSystemTimer() {
    static int guard = 0; // 0x00F8ED80
    if ((guard & 1) == 0) {
        guard |= 1;
        system_timer = gpg::time::Timer{};
    }
    return system_timer;
}

// 0x00955630
LONGLONG gpg::time::MicrosecondsToCycles(LONGLONG micro) {
    initPerformanceCounters();
    LONGLONG freq = PerformanceFrequency.QuadPart;
    return freq * (micro % 1000000 + (micro >> 32) / 1000000);
}

// 0x009555F0
LONGLONG gpg::time::MillisecondsToCycles(float milli) {
    initPerformanceCounters();
    return (LONGLONG) (PerformanceFrequency.QuadPart * milli * 0.001);
}

// 0x009555B0
LONGLONG gpg::time::SecondsToCycles(float sec) {
    initPerformanceCounters();
    return (LONGLONG) (PerformanceFrequency.QuadPart * sec);
}