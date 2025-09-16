#include <Windows.h>

static LARGE_INTEGER PerformanceFrequency; // 0x00F8ED58
static float TimerCycleToSeconds; // 0x00F8ED60
static LONGLONG gpgTime; // 0x00F8ED68
static gpg::time::Timer systemTimer; // 0x00F8ED78

namespace gpg::time {

struct Timer
{
    LONGLONG mTime;

    Timer();      // 0x009556F0 or 0x009556D0
    void Reset(); // 0x009556D0 or 0x009556F0
    LONGLONG ElapsedCyclesAndReset(); // 0x00955710
    LONGLONG ElapsedCycles(); // 0x00955700
    float ElapsedMicroseconds(); // 0x00485A40
    float ElapsedSeconds(); // 0x004A3560
    float ElapsedMilliSeconds() {
        return gpg::time::CyclesToMilliseconds(this->ElapsedCycles());
    } // 0x00461A90
};

LONGLONG GetTime(); // 0x00955400
LONGLONG CyclesToMicroseconds(LONGLONG); // 0x00955520
float CyclesToMilliseconds(LONGLONG); // 0x009554E0
float CyclesToSeconds(LONGLONG); // 0x009554A0
gpg::time::Timer const &GetSystemTimer(); // 0x00955730
LONGLONG MicrosecondsToCycles(LONGLONG); // 0x00955630
LONGLONG MillisecondsToCycles(float); // 0x009555F0
LONGLONG SecondsToCycles(float); // 0x009555B0

}
