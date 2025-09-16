#include "core/resources/AudioEngine.h"
#include "core/HSound.h"
#include "core/SndEntityLoop.h"
#include "boost/shared_ptr.hpp"
#include <string>


static struct_SoundConfig *sSoundConfiguration; // 0x010A9264

namespace Moho {

void SND_Shutdown(); // 0x004D8EE0
bool SND_Enabled(); // 0x004D8F10
void SND_Frame(); // 0x004D8F40
void SND_Mute(bool); // 0x004D8FC0
bool SND_GetGlobalVarIndex(gpg::StrArg, XACTVARIABLEINDEX *); // 0x004D9040
float SND_GetGlobalFloat(XACTVARIABLEINDEX); // 0x004D9090
void SND_SetGlobalFloat(XACTVARIABLEINDEX, float); // 0x004D90E0
boost::shared_ptr<Moho::AudioEngine> SND_FindEngine(gpg::StrArg); // 0x004D9140
Moho::HSndEntityLoop *SND_GetSharedAmbientHandle(Moho::CSndParams *); // 0x004E0140
std::string SND_GetVariableName(int); // 0x004E0150


}
