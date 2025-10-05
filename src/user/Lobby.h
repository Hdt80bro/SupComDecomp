#include "core/MessageReceiver.h"
#include "core/NetConnector.h"
#include "core/ScriptObject.h"
#include "core/TDatList.h"
#include "gpgcore/String.h"
#include <string>
#include <set>
#include <vector>


static const char *sPeerStateStrings[]; // 0x00DFEB14

namespace Moho {

enum EPeerState
{
    PS_Unknown = 0,
    PS_Connecting = 1,
    PS_Connected = 2,
    PS_Pending = 3,
    PS_WaitingJoin = 4,
    PS_Established = 5,
    PS_Disconnected = 6,
    PS_Count = 7,
};

struct SSTICommandSource
{
    int mIndex;
    std::string mName;
    int mTimeouts;
};

    
struct SNetPeer : Moho::TDatListItem<Moho::SNetPeer, void>
{
    std::string mPlayerName;
    int mUID;
    u_long mAddr;
    u_short mPort;
    Moho::EPeerState mState;
    int v2;
    Moho::INetConnection *mPeerConnection;
    std::set<int> mConnectedTo;
    int mCmdSource;
    int mClientInd;

    SNetPeer(const char *name, int id, u_long addr, u_short port, Moho::INetConnection *conn, Moho::EPeerState state); // 0x007C05C0
    std::string ToString(); // 0x007C0690
    ~SNetPeer() = default; // 0x007C1340
    LuaPlus::LuaObject ToLua(LuaPlus::LuaState *state); // 0x007C2950
    void SendInfoTo(Moho::INetConnection *conn); // 0x007C8070
};

class CLobby :
    public Moho::CScriptObject,            // 0x00E3ED34
    public Moho::IMessageReceiver,         // 0x00E3ED48
    public Moho::INetDatagramHandler,      // 0x00E3ED50
    public Moho::CPushTask<Moho::CLobby>,  // 0x00E3ED58
    public Moho::CPullTask<Moho::CLobby>   // 0x00E3ED64
{
public:
    Moho::INetConnector *mConnector;
    int mMaxConnections;
    HANDLE mEvent;
    bool mJoinedLobby;
    Moho::INetConnection *mPeerConnection;
    int mHasNAT;
    std::string mPlayerName;
    int mLocalUID;
    Moho::TDatList<Moho::SNetPeer, void> mPlayers;
    bool mReady;
    int mNextId;
    std::auto_ptr<Moho::INetDatagramSocket> mSocket;
    int mTime1;

    static gpg::RType *StaticGetClass(); // 0x007C0760
    gpg::RType *GetClass() const override; // 0x007C0780
    gpg::RRef GetDerivedObjectRef() override; // 0x007C07A0
    ~CLobby() override; // 0x007C0C60
    void ReceiveMessage(Moho::CMessage *dat, Moho::CMessageDispatcher *disp) override; // 0x007C62F0
    void HandleMessage(Moho::CMessage *msg, Moho::INetDatagramSocket *sock, u_long addr, u_short port) override; // 0x007C5840
    //~CPushTask<Moho::CLobby>() override; // 0x007D0050
    void Push() override; // 0x007C8CB0 -> 0x007C5490
    //~CPullTask<Moho::CLobby>() override; // 0x007D0050
    void Pull() override; // 0x007C8BF0 -> 0x007C56B0

    std::string MakeValidPlayerName(std::string &name, int id); // 0x007C1720
    void HostGame(); // 0x007C1B20
    void JoinGame(const char *, const char *playername, int playerId); // 0x007C1DA0
    void BroadcastScriptData(const LuaPlus::LuaObject &dat); // 0x007C2210
    void SendScriptData(int id, const LuaPlus::LuaObject &dat); // 0x007C24C0
    LuaPlus::LuaObject GetPeers(LuaPlus::LuaState *state); // 0x007C27E0
    void LaunchGame(const LuaPlus::LuaObject &dat); // 0x007C38C0
    void AssignClientIndex(int, gpg::StrArg, __inout unsigned int &, __inout int &); // 0x007C4E80
    unsigned int AssignCommandSource(int, int, std::vector<Moho::SSTICommandSource> &, __inout unsigned int &); // 0x007C4F60
    void ConnectionFailed(Moho::INetConnection *conn); // 0x007C5B60
    void ConnectionMade(Moho::CMessageStream *strm, Moho::INetConnection *conn); // 0x007C5CA0
    void Reconnect(Moho::INetConnection *conn); // 0x007C5ED0
    void PeerJoined(Moho::CMessage *msg, Moho::INetConnection *conn); // 0x007C64C0
    void Ejected(Moho::CMessage *msg); // 0x007C6AD0
    void Joined(Moho::CMessage *msg, Moho::INetConnection *conn); // 0x007C6BD0
    void DataReceived(Moho::CMessage *msg, Moho::INetConnection *conn); // 0x007C6EE0
    void NewPeer(Moho::CMessage *msg, Moho::INetConnection *conn); // 0x007C7010
    void ConnectToPeer(LuaPlus::LuaState *state, const char *, const char *name, int id); // 0x007C7190
    void DeletePeer(Moho::CMessage *msg, Moho::INetConnection *conn); // 0x007C76A0
    void DisconnectFromPeer(int id); // 0x007C7790
    void PeerDisconnected(Moho::SNetPeer *plyr); // 0x007C77F0
    void EjectPeer(int id, const char *); // 0x007C7990
    void KickPeer(Moho::SNetPeer *plyr, const char *); // 0x007C7AC0
    void EstablishedPeers(Moho::CMessage *msg, Moho::INetConnection *); // 0x007C7C10
    void SystemMessage(...); // 0x007C7FC0
    void BroadcastStream(Moho::SDataView *msg); // 0x007C8040
};

}

std::string func_PeerStateToString(Moho::EPeerState state); // 0x007C0560
