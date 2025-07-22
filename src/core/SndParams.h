#include <string>
#include "boost/shared_ptr.hpp"
#include "SndVar.h"

namespace Moho {

class AudioEngine;

struct CSndParams
{
    std::string cue1;
    std::string bank;
    Moho::CSndVar *lodCutoff;
    Moho::CSndVar *cue2;
    int v1;
    __int16 cueId;
    __int16 bankId;
    boost::shared_ptr<Moho::AudioEngine> engine;
};

}
