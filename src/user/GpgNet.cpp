#include "GpgNet.h"
#include "user/Wld.h"

boost::shared_ptr<Moho::CGpgNetInterface> sGPGNet; // 0x010BE118


// 0x007B9360
void Moho::GPGNET_Attach(u_long addr, u_short port) {
    if (sGPGNet) {
        throw std::runtime_error{std::string{"Can't attach to a gpg.net if we already are."}};
    }
    boost::shared_ptr<Moho::CGpgNetInterface> ptr{new Moho::CGpgNetInterface{}};
    ptr->Connect(addr, port);
    func_SetGpgNet(ptr);
}

// 0x007B94C0
void Moho::GPGNET_ReportBottleneck(const Moho::SClientBottleneckInfo &info) {
    boost::shared_ptr<Moho::CGpgNetInterface> ptr{sGPGNet};
    if (ptr && Moho::WLD_GetDriver() != nullptr) {
        Moho::CClientManagerImpl *manager = Moho::WLD_GetDriver()->GetClientManager();
        const char *typeStr;
        switch (info.mType) {
            case Moho::SClientBottleneckInfo::Type::Nothing: {
                typeStr = "nothing";
                break;
            }
            case Moho::SClientBottleneckInfo::Type::Readiness: {
                typeStr = "readiness";
                break;
            }
            case Moho::SClientBottleneckInfo::Type::Data: {
                typeStr = "data";
                break;
            }
            case Moho::SClientBottleneckInfo::Type::Ack: {
                typeStr = "ack";
                break;
            }
            default: {
                typeStr = "unknown";
                break;
            }
        }
        std::string str{};
        unsigned int next = info.mSubobj.GetNext(-1);
        if (next != info.mSubobj.Max()) {
            str = gpg::STR_Printf("%u", manager->GetClient(next)->mLaunchInfo);
            next = info.mSubobj.GetNext(next);
            while (next != info.mSubobj.Max()) {
                str.append(gpg::STR_Printf(",%u", manager->GetClient(next)->mLaunchInfo));
                next = info.mSubobj.GetNext(next);
            }
        }
        Moho::SNetCommandArg timeArg{gpg::STR_Printf("%.1f", info.mMillis)};
        Moho::SNetCommandArg srcsArg{str};
        Moho::SNetCommandArg beatArg{gpg::STR_Printf("%u", info.mBeat)};
        Moho::SNetCommandArg typeArg{typeStr};
        ptr->WriteCommand4(
            "Bottleneck",
            &typeArg,
            &beatArg,
            &srcsArg,
            &timeArg
        );
    }
}

// 0x007B9A20
void Moho::GPGNET_ReportBottleneckCleared() {
    boost::shared_ptr<Moho::CGpgNetInterface> ptr{sGPGNet};
    if (ptr) {
        ptr->SendBottleneckCleared();
    }
}

// 0x007B9AC0
void Moho::GPGNET_ReportDesync(int a1, int a2, const std::string &a3, const std::string &a4) {
    boost::shared_ptr<Moho::CGpgNetInterface> ptr{sGPGNet};
    if (ptr) {
        Moho::SNetCommandArg arg4{a4};
        Moho::SNetCommandArg arg3{a3};
        Moho::SNetCommandArg arg2{a2};
        Moho::SNetCommandArg arg1{a1};
        ptr->WriteCommand4("Desync", &arg1, &arg2, &arg3, &arg4);
    }
}

// 0x007B9CD0
void Moho::GPGNET_SubmitArmyStats(const std::string &stat) {
    boost::shared_ptr<Moho::CGpgNetInterface> ptr{sGPGNet};
    if (ptr) {
        Moho::SNetCommandArg com{stat};
        ptr->WriteCommand1("Stats", &com);
    }
}

// 0x007B9DD0 -> 0x007BB590
void Moho::GPGNET_Shutdown() {
    sGPGNet.reset();
}

// 0x007B9470
void func_SetGpgNet(boost::shared_ptr<Moho::CGpgNetInterface> &ptr) {
    sGPGNet = ptr;
}
