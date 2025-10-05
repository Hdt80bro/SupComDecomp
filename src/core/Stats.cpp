// known file

#include "Stats.h"
#include "gpgcore/String.h"

// 0x008D1040
std::string func_FmtByteSize(size_t amt) {
    if (amt + 0x400 <= 0x800) {
        return gpg::STR_Printf("%dB", amt);
    }
    const char *unit = "KMG";
    int adjusted = amt;
    while (abs(adjusted) >= 1024*1024) {
        ++unit;
        adjusted /= 1024;
    }
    double val = adjusted / 1024.0;
    if (fabs(val) < 10.0) {
        return gpg::STR_Printf("%.3f%c", val, *unit);
    } else if (fabs(val) < 100.0) {
        return gpg::STR_Printf("%.2f%c", val, *unit);
    } else {
        return gpg::STR_Printf("%.1f%c", val, *unit);
    }
}
