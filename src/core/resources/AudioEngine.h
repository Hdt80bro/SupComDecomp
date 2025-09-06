#include "core/Math.h"
#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"
#include "Wm3Vector3.h"
#include "boost/shared_ptr.hpp"
#include <stdexcept>

namespace Moho {

// 0x00E0B718
class XAudioError : public std::runtime_error
{
public:
    ~XAudioError() override = default; // 0x004D85D0
};

struct AudioEngineImpl
{
    
};

struct AudioEngine
{
    Moho::AudioEngineImpl *mImpl;

    const Moho::AudioEngineImpl &GetImpl() const; // MohoEngine.dll
    Moho::AudioEngineImpl &GetImpl(); // MohoEngine.dll
    boost::shared_ptr<Moho::AudioEngine> Create(gpg::StrArg); // 0x004D9340
    void Shutdown(); // 0x004D93F0
    AudioEngine(gpg::StrArg); // 0x004D9410
    ~AudioEngine(); // 0x004D9760
    Moho::VTransform GetListenerTransform(); // 0x004D9780
    void SetListenerTransform(const Moho::VTransform &); // 0x004D9890
    void Calculate3D(IXACTCue *, const Wm3::Vector3f &, const Wm3::Vector3f &); // 0x004D9A60
    long Play(unsigned short, unsigned short, IXACTCue **); // 0x004D9B30
    bool GetBankIndex(gpg::StrArg, unsigned short *); // 0x004D9BD0
    bool GetCueIndex(gpg::StrArg, unsigned short, unsigned short *); // 0x004D9C40
    void SetPaused(gpg::StrArg, bool); // 0x004D9C90
    void SetVolume(gpg::StrArg, float); // 0x004D9DB0
    float GetVolume(gpg::StrArg); // 0x004D9E50
    bool GetPaused(gpg::StrArg); // 0x004D9EC0
    void StopAllSounds(gpg::StrArg); // 0x004D9F50
    bool IsStopped(IXACTCue *); // 0x004D9FA0
};

}

struct struct_SoundLoader
{
    gpg::MemBuffer<char> mBuff;
    IXACTSoundBank *mBank;
    std::string mStr;
    Moho::AudioEngineImpl *mEngine;

    bool Load(const char *); // 0x004DA8E0
};


int func_CreateXACTinstance(unsigned int mode, LPVOID *dest); // 0x004D8090
int func_AudioInitialize(IXACTEngine *engine, X3DAUDIO_HANDLE *instance); // 0x004D8170
void func_RetreiveXACTCOMInterface(Moho::AudioEngineImpl *engine); // 0x004D8970
const char *func_SoundErrorCodeToMsg(unsigned int a1); // 0x004D8A50
void func_LoadSoundPath(Moho::AudioEngineImpl *engine, const char *voicePath); // 0x004DA500
