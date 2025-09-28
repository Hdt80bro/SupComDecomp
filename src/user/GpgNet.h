#include "user/Client.h"
#include "core/NetTCPConn.h"
#include "core/Set.h"
#include "core/Task.h"
#include "LuaPlus.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include <deque>
#include <vector>

namespace Moho {

struct SNetCommandArg
{
    enum Type
    {
        NETARG_Num = 0x0,
        NETARG_String = 0x1,
        NETARG_Data = 0x2,
    };

    Moho::SNetCommandArg::Type mType;
    int mNum;
    std::string mStr;

    SNetCommandArg(int num) :
        mType{NETARG_Num},
        mNum{num},
        mStr{}
    {}
    SNetCommandArg(std::string str) :
        mType{NETARG_String},
        mNum{0},
        mStr{str}
    {}
};

struct SNetCommand
{
    std::string mName;
    std::vector<Moho::SNetCommandArg> mArgs;
    int mVal;
};

class CGpgNetInterface :
    public Moho::CPullTask<Moho::CGpgNetInterface>, // 0x00E3D74C
    public Moho::INetNATTraversalProvider           // 0x00E3D758
{
public:
    boost::shared_ptr<Moho::CGpgNetInterface> mSelf;
    gpg::Mutex mLock;
    DWORD mState;
    Moho::INetTCPServer *mServer;
    Moho::INetTCPSocket *mSocket;
    std::deque<Moho::SNetCommand> mCommands;
    HANDLE mEvent;
    boost::thread *mNatHandler;
    LuaPlus::LuaObject mLObj;
    boost::weak_ptr<Moho::INetNATTraversalHandler> mNATHandler;

    ~CGpgNetInterface() override; // 0x007B68C0
    int Execute() override; // 0x007BB250
    void SetHandler(int port, boost::shared_ptr<Moho::INetNATTraversalHandler> *) override; // 0x007B9070
    void ReceivePacket(u_long addr, u_short port, const char *dat, size_t size); // 0x007B9160

    void ExpectedInt() noexcept(false); // 0x007B65C0
    void ExpectedData() noexcept(false); // 0x007B66B0
    void EnqueueCommand0(const char *str, int val); // 0x007B67A0
    CGpgNetInterface(); // 0x007B6800
    void Connect(u_long addr, u_short port); // 0x007B6A30
    void WriteCommand(const char *name, std::vector<Moho::SNetCommandArg> &args); // 0x007B6DB0
    void SendBottleneckCleared(); // 0x007B6F00
    void WriteCommand1(const char *name, Moho::SNetCommandArg *arg); // 0x007B6FF0
    void WriteCommand2(/*argument absorbed as "ProcessNatPacket"*/const char *name, Moho::SNetCommandArg *arg1, Moho::SNetCommandArg *arg2); // 0x007B70F0
    void WriteCommand3(const char *name, Moho::SNetCommandArg *arg1, Moho::SNetCommandArg *arg2, Moho::SNetCommandArg *arg3); // 0x007B7200
    void WriteCommand4(const char *name, Moho::SNetCommandArg *arg1, Moho::SNetCommandArg *arg2, Moho::SNetCommandArg *arg3, Moho::SNetCommandArg *arg4); // 0x007B7310
    void WriteCommandName(const char *name); // 0x007B7420
    void WriteString(const char *str); // 0x007B74A0
    void WriteArg(Moho::SNetCommandArg *arg); // 0x007B7520
    void Process(); // 0x007B7710
    void Test(std::vector<Moho::SNetCommandArg> &args); // 0x007B7A30
    void Connected(std::vector<Moho::SNetCommandArg> &args); // 0x007B7C50
    void CreateLobby(std::vector<Moho::SNetCommandArg> &args); // 0x007B7DE0
    void HostGame(std::vector<Moho::SNetCommandArg> &args); // 0x007B81D0
    void JoinGame(std::vector<Moho::SNetCommandArg> &args); // 0x007B83C0
    void ConnectToPeer(std::vector<Moho::SNetCommandArg> &args); // 0x007B85A0
    void DisconnectFromPeer(std::vector<Moho::SNetCommandArg> &args); // 0x007B8780
    void HasSupcom(std::vector<Moho::SNetCommandArg> &args); // 0x007B8920
    void HasForgedAlliance(std::vector<Moho::SNetCommandArg> &args); // 0x007B8A70
    void SendNatPacket(std::vector<Moho::SNetCommandArg> &args); // 0x007B8BC0
    void EjectPlayer(std::vector<Moho::SNetCommandArg> &args); // 0x007B8E20
    void ConnectThread(u_long addr, u_short port); // 0x007BA5E0
    void ReadFromScoket(); // 0x007BA880
    void EnqueueCommand(const char *name, std::vector<Moho::SNetCommandArg> &args, int val); // 0x007BAE50
};


void GPGNET_Attach(u_long addr, u_short port); // 0x007B9360
void GPGNET_ReportBottleneck(const Moho::SClientBottleneckInfo &); // 0x007B94C0
void GPGNET_ReportBottleneckCleared(); // 0x007B9A20
void GPGNET_ReportDesync(int, int, const std::string &, const std::string &); // 0x007B9AC0
void GPGNET_SubmitArmyStats(const std::string &); // 0x007B9CD0
void GPGNET_Shutdown(); // 0x007B9DD0 -> 0x007BB590

}

static boost::shared_ptr<Moho::CGpgNetInterface> sGPGNet; // 0x010BE118

void func_SetGpgNet(boost::shared_ptr<Moho::CGpgNetInterface> &); // 0x007B9470
