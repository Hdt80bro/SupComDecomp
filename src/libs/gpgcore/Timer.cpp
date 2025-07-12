#include "Timer.h"

LARGE_INTEGER PerformanceFrequency;
float TimerCycleToSeconds;
LONGLONG gpg_time;
gpg::time::Timer system_timer; // first set at 0x00BEAB90

inline void initPerformanceCounters() {
    if (! PerformanceFrequency.QuadPart) {
        QueryPerformanceCounter(&PerformanceFrequency);
        TimerCycleToSeconds = 1.0 / PerformanceFrequency.QuadPart;
    }
}

gpg::time::Timer::Timer() :
    time{gpg::time::GetTime()}
{}

LONGLONG gpg::time::Timer::ElapsedCyclesAndReset() {
    LONGLONG curTime = gpg::time::GetTime();
    LONGLONG diff = curTime - this->time;
    this->time = curTime;
    return diff;
}
LONGLONG gpg::time::Timer::ElapsedCycles() {
    return gpg::time::GetTime() - this->time;
}
float gpg::time::Timer::ElapsedMicroseconds() {
    return gpg::time::CyclesToMicroseconds(this->ElapsedCycles());
}
float gpg::time::Timer::ElapsedSeconds() {
    return gpg::time::CyclesToSeconds(this->ElapsedCycles());
}

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
LONGLONG gpg::time::CyclesToMicroseconds(LONGLONG cycles) {
    initPerformanceCounters();
    LONGLONG freq = PerformanceFrequency.QuadPart;
    return 1000000 * (cycles % freq + (cycles >> 32) / freq);
}
float gpg::time::CyclesToMilliseconds(LONGLONG cycles) {
    initPerformanceCounters();
    return cycles * TimerCycleToSeconds * 1000.0;
}
float gpg::time::CyclesToSeconds(LONGLONG cycles) {
    initPerformanceCounters();
    return cycles * TimerCycleToSeconds;
}
gpg::time::Timer const &gpg::time::GetSystemTimer() {
    static int guard = 0; // 0x00F8ED80
    if ((guard & 1) == 0) {
        guard |= 1;
        system_timer = gpg::time::Timer{};
    }
    return system_timer;
}
LONGLONG gpg::time::MicrosecondsToCycles(LONGLONG micro) {
    initPerformanceCounters();
    LONGLONG freq = PerformanceFrequency.QuadPart;
    return freq * (micro % 1000000 + (micro >> 32) / 1000000);
}
LONGLONG gpg::time::MillisecondsToCycles(float milli) {
    initPerformanceCounters();
    return (LONGLONG) (PerformanceFrequency.QuadPart * milli * 0.001);
}
LONGLONG gpg::time::SecondsToCycles(float sec) {
    initPerformanceCounters();
    return (LONGLONG) (PerformanceFrequency.QuadPart * sec);
}