#include "core/resources/AudioEngine.h"
#include <string>

namespace Moho {

// 0x00E0B6F4
class RWaveBankResource
{
public:
    int v1;
    std::string mStr;
    Moho::AudioEngine mEngine;

    virtual ~RWaveBankResource() = default; // 0x004D83A0
    virtual bool Load() = 0;
};

// 0x00E0B70C
class RWaveBankResStreaming : public Moho::RWaveBankResource
{
public:
    HANDLE mHandle;

    ~RWaveBankResStreaming() override; // 0x004D8460
    bool Load() override; // 0x004DADF0
};

// 0x00E0B700
class RWaveBankResInMemory : public Moho::RWaveBankResource
{
public:
    int v10;
    int v11;
    int v12;
    int v13;

    ~RWaveBankResInMemory() override; // 0x004D8410
    bool Load() override; // 0x004DAB70
};


}
