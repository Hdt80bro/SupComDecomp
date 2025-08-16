#include "core/SndVar.h"
#include "boost/shared_ptr.hpp"
#include <string>

namespace Moho {

class AudioEngine;

struct CSndParams
{
    std::string mCue;
    std::string mBank;
    Moho::CSndVar *mLODCutoff;
    Moho::CSndVar *mVar2;
    int v1;
    __int16 mCueId;
    __int16 mBankId;
    boost::shared_ptr<Moho::AudioEngine> mAudioEngine;
};

}
