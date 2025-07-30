#include <string>
#include "boost/shared_ptr.hpp"
#include "core/AudioEngine.h"
#include "core/HSound.h"

namespace Moho {

void SND_Shutdown(); // 0x004D8EE0
bool SND_Enabled(); // 0x004D8F10
void SND_Frame(); // 0x004D8F40
void SND_Mute(bool); // 0x004D8FC0
bool SND_GetGlobalVarIndex(const char *, unsigned short *); // 0x004D9040
float SND_GetGlobalFloat(unsigned short); // 0x004D9090
void SND_SetGlobalFloat(unsigned short, float); // 0x004D90E0
boost::shared_ptr<Moho::AudioEngine> SND_FindEngine(const char *); // 0x004D9140
Moho::HSndEntityLoop *SND_GetSharedAmbientHandle(Moho::CSndParams *); // 0x004E0140
std::string SND_GetVariableName(int); // 0x004E0150


}
