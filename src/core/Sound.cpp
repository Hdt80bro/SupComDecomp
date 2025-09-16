#include "Sound.h"


struct_SoundConfig *sSoundConfiguration; // 0x010A9264

// 0x004D8EE0
void Moho::SND_Shutdown() {
    struct_SoundConfig *old = sSoundConfiguration;
    sSoundConfiguration = nullptr;
    if (old != nullptr) {
        delete(old);
    }
}

// 0x004D8F10
bool Moho::SND_Enabled() {
    return ! sSoundConfiguration->mEngines.empty();
}

// 0x004D8F40
void Moho::SND_Frame() {
    for (int i = 0; i < sSoundConfiguration->mEngines.size(); ++i) {
        IXACT3Engine *inst = sSoundConfiguration->mEngines[i]->mImpl->mInstance;
        if (inst != nullptr) {
            inst->DoWork();
        }
    }
    sSoundConfiguration->mTime.Reset();
}

// 0x004D8FC0
void Moho::SND_Mute(bool mute) {
    for (int i = 0; i < sSoundConfiguration->mEngines.size(); ++i) {
        Moho::AudioEngineImpl *engine = sSoundConfiguration->mEngines[i]->mImpl;
        if (engine->mInstance != nullptr) {
            float vol;
            if (mute) {
                vol = 0.0;
                engine->mGlobVol = engine->mEngine->GetVolume("Global");
            } else {
                vol = engine->mGlobVol;
            }
            engine->mEngine->SetVolume("Global", vol);
        }
    }
}

// 0x004D9040
bool Moho::SND_GetGlobalVarIndex(gpg::StrArg name, XACTVARIABLEINDEX *index) {
    if (sSoundConfiguration->mEngines.empty() || sSoundConfiguration->mNoSound) {
        return false;
    }
    IXACT3Engine *engine = sSoundConfiguration->mEngines[0]->mImpl->mInstance;
    if (engine == nullptr) {
        return false;
    }
    XACTVARIABLEINDEX i = engine->GetGlobalVariableIndex(name);
    *index = i;
    return i != XACTVARIABLEINDEX_INVALID;
}

// 0x004D9090
float Moho::SND_GetGlobalFloat(XACTVARIABLEINDEX index) {
    if (sSoundConfiguration->mEngines.empty() || sSoundConfiguration->mNoSound) {
        return NaN;
    }
    IXACT3Engine *engine = sSoundConfiguration->mEngines[0]->mImpl->mInstance;
    if (engine == nullptr) {
        return NaN;
    }
    XACTVARIABLEVALUE val;
    if (engine->GetGlobalVariable(index, &val) < 0) {
        return NaN;
    }
    return val;
}

// 0x004D90E0
void Moho::SND_SetGlobalFloat(XACTVARIABLEINDEX index, float val) {
    if (sSoundConfiguration->mEngines.empty() || sSoundConfiguration->mNoSound) {
        return;
    }
    IXACT3Engine *engine = sSoundConfiguration->mEngines[0]->mImpl->mInstance;
    if (engine == nullptr) {
        return;
    }
    if (engine->SetGlobalVariable(index, val) < 0) {
        gpg::Warnf("SND: Error setting global variable [index:%i]", index);
    }
}

// 0x004D9140
boost::shared_ptr<Moho::AudioEngine> Moho::SND_FindEngine(gpg::StrArg bank) {
    if (sSoundConfiguration != nullptr && ! sSoundConfiguration->mEngines.empty()) {
        for (int i = 0; i < sSoundConfiguration->mEngines.size(); ++i) {
            Moho::AudioEngineImpl *engine = sSoundConfiguration->mEngines[i]->mImpl;
            if (! engine->mBanks.empty()) {
                for (int k = 0; k < engine->mBanks.size(); ++k) {
                    if (stricmp(engine->mBanks[k]->str1.c_str(), bank) != 0) {
                        return sSoundConfiguration->mEngines[i];
                    }
                }
            }
        }
    }
    return boost::shared_ptr<Moho::AudioEngine>{};
}

// 0x004E0140
Moho::HSndEntityLoop *Moho::SND_GetSharedAmbientHandle(Moho::CSndParams *) {

}

// 0x004E0150
std::string Moho::SND_GetVariableName(int) {

}
