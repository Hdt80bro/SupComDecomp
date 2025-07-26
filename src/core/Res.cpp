#include "Res.h"
#include "File.h"

std::map<std::string, gpg::RType *> *pPrefetchTypeMap; // 0x010A6390


// 0x004A5060
void Moho::RES_RegisterPrefetchType(const char *key, gpg::RType *type) {
    (*func_GetPrefetchTypeMap())[std::string{key}] = type;
}

// 0x004A9350
std::string Moho::RES_CompletePath(gpg::StrArg dir, gpg::StrArg name) {
    std::string prefix = Moho::FILE_DirPrefix(name, false);
    std::string abs = Moho::FILE_MakeAbsolute(dir, prefix.c_str());
    bool res = 0;
    return Moho::FILE_CollapsePath(abs.c_str(), &res);
}


// 0x004A4FB0
std::map<std::string, gpg::RType *> *func_GetPrefetchTypeMap() {
    if (pPrefetchTypeMap == nullptr) {
        static std::map<std::string, gpg::RType *> prefetchTypeMap{}; // 0x010C7940
        pPrefetchTypeMap = &prefetchTypeMap;
    }
    return pPrefetchTypeMap;
}
