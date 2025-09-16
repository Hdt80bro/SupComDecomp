#include "core/Math.h"
#include "gpgcore/MemBuffer.h"
#include "gpgcore/String.h"
#include "gpgcore/Timer.h"
#include "Wm3Vector3.h"
#include "boost/shared_ptr.hpp"
#include ""
#include <stdexcept>
#include <vector>


struct struct_SoundConfig
{
    gpg::time::Timer mTime;
    std::vector<boost::shared_ptr<Moho::AudioEngine>> mEngines;
    gpg::MemBuffer<char> mBuff;
    bool mNoSound;
    BYTE b1;
    BYTE b2;
    BYTE b3;
    DWORD mConfiguration;
    DWORD mLibrary;
    DWORD mVolume1;
    DWORD mStart;
    DWORD mLen;
    DWORD dword40;
    DWORD dword44;
    DWORD dword48;
    DWORD dword4C;
    DWORD (*mHandleSoundEvent)(int *);
    DWORD dword54;
    bool mAudition;
    BYTE b4;
    BYTE b5;
    BYTE b6;
    DWORD v2;

    ~struct_SoundConfig(); // 0x004D9250
};

namespace Moho {

// 0x00E0B718
class XAudioError : public std::runtime_error
{
public:
    ~XAudioError() override = default; // 0x004D85D0
};

struct AudioEngineImpl
{
    struct_SoundConfig *mConfigs;
    Moho::AudioEngine *mEngine;
    std::vector<IXACT3SoundBanks> mBanks;
    DWORD v6;
    DWORD v7;
    DWORD v8;
    DWORD v9;
    std::map<unk_t, unk_t> mMap1;
    IXACT3Engine *mInstance;
    X3DAUDIO_LISTENER mListener;
    std::map<const char *, float> mMap2;
    float mGlobVol;
    X3DAUDIO_DSP_SETTINGS mSettings;
    X3DAUDIO_EMITTER mEmitter;
    X3DAUDIO_HANDLE mAudioHandle;
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
    void Calculate3D(IXACT3Cue *, const Wm3::Vector3f &, const Wm3::Vector3f &); // 0x004D9A60
    long Play(unsigned short, unsigned short, IXACTCue **); // 0x004D9B30
    bool GetBankIndex(gpg::StrArg, unsigned short *); // 0x004D9BD0
    bool GetCueIndex(gpg::StrArg, unsigned short, unsigned short *); // 0x004D9C40
    void SetPaused(gpg::StrArg, bool); // 0x004D9C90
    void SetVolume(gpg::StrArg, float); // 0x004D9DB0
    float GetVolume(gpg::StrArg); // 0x004D9E50
    bool GetPaused(gpg::StrArg); // 0x004D9EC0
    void StopAllSounds(gpg::StrArg); // 0x004D9F50
    bool IsStopped(IXACT3Cue *); // 0x004D9FA0
};

}

struct struct_SoundLoader
{
    gpg::MemBuffer<char> mBuff;
    IXACT3SoundBank *mBank;
    std::string mStr;
    Moho::AudioEngineImpl *mEngine;

    bool Load(const char *); // 0x004DA8E0
};


int func_CreateXACTinstance(unsigned int mode, LPVOID *dest); // 0x004D8090
int func_AudioInitialize(IXACT3Engine *engine, X3DAUDIO_HANDLE *instance); // 0x004D8170
void func_RetreiveXACTCOMInterface(Moho::AudioEngineImpl *engine); // 0x004D8970
const char *func_SoundErrorCodeToMsg(unsigned int a1); // 0x004D8A50
void func_LoadSoundPath(Moho::AudioEngineImpl *engine, const char *voicePath); // 0x004DA500
