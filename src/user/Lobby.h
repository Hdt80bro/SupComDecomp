#include "core/MessageReceiver.h"
#include "core/NetConnector.h"
#include "core/ScriptObject.h"
#include "core/TDatList.h"
#include <string>
#include <vector>


struct struct_NetworkPlayer : Moho::TDatListItem<struct_NetworkPlayer, void>
{
    std::string mPlayerName;
    int mUID;
    u_long mAddr;
    u_short mPort;
    int mNetworkType;
    int v2;
    int mPeerConnection;
    std::vector<unk_t> mMap;
    int mCtr;
};


namespace Moho {

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
    Moho::TDatList<struct_NetworkPlayer, void> mPlayers;
    bool v36;
    int v37;
    int v38;
    int v39;

    static gpg::RType *StaticGetClass(); // 0x007C0760
    gpg::RType *GetClass() const override; // 0x007C0780
    gpg::RRef GetDerivedObjectRef() override; // 0x007C07A0
    ~CLobby() override; // 0x007C0C60
    // ...
};

}
