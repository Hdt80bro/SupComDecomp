// known file

#include "Lobby.h"
#include "user/Wld.h"
#include "core/Localization.h"
#include "core/Set.h"
#include "gpgcore/General.h"
#include "gpgcore/streams/BinaryReader.h"
#include "LuaPlus.h"
#include <charconv>

const char *sPeerStateStrings[] = { // 0x00DFEB14
    "Unknown",
    "Connecting",
    "Connected",
    "Pending",
    "WaitingJoin",
    "Established",
    "Disconnected"
};


// 0x007C05C0
Moho::SNetPeer::SNetPeer(const char *name, int id, u_long addr, u_short port, Moho::INetConnection *conn, Moho::EPeerState state) :
    Moho::TDatListItem<Moho::SNetPeer, void>{},
    mPlayerName{name},
    mUID{id},
    mAddr{addr},
    mPort{port},
    mState{state},
    v2{0},
    mPeerConnection{conn},
    mConnectedTo{},
    mCmdSource{255},
    mClientInd{-1}
{}

// 0x007C0690
std::string Moho::SNetPeer::ToString() {
    return gpg::STR_Printf("\"%s\" [%s:%d, uid=%d]",
        this->mPlayerName.c_str(),
        Moho::NET_GetHostName(this->mAddr).c_str(),
        this->mPort,
        this->mUID
    );
}

// 0x007C2950
LuaPlus::LuaObject Moho::SNetPeer::ToLua(LuaPlus::LuaState *state) {
    LuaPlus::LuaObject obj{};
    obj.AssignNewTable(state, 0, 0);
    obj.SetString("name", this->mPlayerName.c_str());
    char buff[28/*at least*/];
    std::to_chars(this->mUID, buff);
    obj.SetString("status", func_PeerStateToString(this->mState).c_str());
    obj.SetNumber("ping", this->mPeerConnection->GetPing());
    obj.SetNumber("quiet", this->mPeerConnection->GetTime());
    LuaPlus::LuaObject est{};
    est.AssignNewTable(state, 0, 0);
    int k = 1;
    for (auto i = this->mConnectedTo.begin(); i != this->mConnectedTo.end(); ++i) {
        std::to_chars(*i, buff);
        est.SetString(k++, buff);
    }
    obj.SetObject("establishedPeers", est);
    return obj;
}

// 0x007C8070
void Moho::SNetPeer::SendInfoTo(Moho::INetConnection *conn) {
    gpg::Logf("LOBBY: sending info on peer %s to %s", this->ToString().c_str(), conn->ToString().c_str());
    Moho::CMessage msg{0, LOBMSG_NewPeer};
    Moho::CMessageStream strm{&msg};
    strm.Write(this->mPlayerName);
    strm.Write<u_long>(this->mAddr);
    strm.Write<u_short>(this->mPort);
    strm.Write<int>(this->mUID);
    conn->Write(&msg);
}


DEFINE_ROBJECT_COMMON(Moho::CLobby);

// 0x007C0C60
Moho::CLobby::~CLobby() {
    if (this->mSocket != nullptr) {
        Moho::WIN_GetWaitHandleSet()->RemoveHandle(this->mSocket->CreateEvent());
    }
    if (this->mConnector != nullptr) {
        delete(this->mConnector);
        this->mConnector = nullptr;
    }
    Moho::WIN_GetWaitHandleSet()->RemoveHandle(this->mEvent);
    ::CloseHandle(this->mEvent);
}

// 0x007C62F0
void Moho::CLobby::ReceiveMessage(Moho::CMessage *msg, Moho::CMessageDispatcher *disp) {
    auto conn = static_cast<Moho::INetConnection *>(disp);
    switch (msg->GetType()) {
        case LOBMSG_ConnectionEstablished: {
            this->PeerJoined(msg, conn);
            break;
        }
        case LOBMSG_KickPeer: {
            if (this->mPeerConnection == conn) {
                this->Ejected(msg);
            } else {
                gpg::Logf("LOBBY: ignoring Rejected message from non-host.");
            }
            break;
        }
        case LOBMSG_PeerJoined: {
            this->Joined(msg, conn);
            break;
        }
        case LOBMSG_NewPeer: {
            this->NewPeer(msg, conn);
            break;
        }
        case LOBMSG_DeletePeer: {
            this->DeletePeer(msg, conn);
            break;
        }
        case LOBMSG_EstablishedPeers: {
            this->EstablishedPeers(msg, conn);
        }
        case LOBMSG_BroadcastScript:
        case LOBMSG_SendScript: {
            this->DataReceived(msg, conn);
            break;
        }
        case CONMSG_ConnectionFailed: {
            this->ConnectionFailed(conn);
            break;
        }
        case CONMSG_ConnectionMade: {
            this->ConnectionMade(conn);
            break;
        }
        case CONMSG_2:
        case CONMSG_3: {
            this->Reconnect(conn);
            break;
        }
        default: {
            gpg::Logf("LOBBY: Ignoring unrecognized lobby message w/ type %d", dat->GetType());
            break;
        }
    }
}

// 0x007C5840
void Moho::CLobby::HandleMessage(Moho::CMessage *msg, Moho::INetDatagramSocket *, u_long addr, u_short port) {
    if (msg->GetType() != DISCOVERMSG_Request) {
        gpg::Logf(
            "LOBBY: ignoring unexpected message type (%d) from %s:%d",
            msg->GetType(),
            Moho::NET_GetHostName(addr).c_str(),
            port
        );
    }
    gpg::Logf("LOBBY: received discovery request from %s:%d", Moho::NET_GetHostName(addr).c_str(), port);
    LuaPlus::LuaObject obj = this->RunScript("GameConfigRequested");
    Moho::CMessage reply{0, DISCOVERMSG_Response};
    Moho::CMessageStream strm{&reply};
    strm.Write<char>(11);
    strm.Write<char>(1);
    strm.Write<char>(0);
    strm.Write<char>(this->mConnector->GetProtocol());
    strm.Write<u_short>(this->mConnector->GetLocalPort());
    if (Moho::SCR_ToByteStream(obj, strm)) {
        this->mSocket->Send(&reply, addr, port);
    } else {
        gpg::Warnf("Error serializing lua game config.");
    }
}

// 0x007C8CB0 -> 0x007C5490
void Moho::CLobby::Push() {
    if (this->mSocket != nullptr) {
        this->mSocket->Pull();
    }
    ResetEvent(this->mEvent);
    if (this->mConnector == nullptr) {
        return;
    }
    u_long addr;
    u_short port;
    while (this->mConnector->Func2(&addr, &port)) {
        if (! this->mHasNAT || this->mPeerConnection != nullptr) {
            this->mConnector->Reject(addr, port);
            gpg::Logf("LOBBY: rejecting unexpected connection from %s:%d", Moho::NET_GetHostName(addr).c_str(), port);
        } else {
            Moho::INetConnection *conn = this->mConnector->Accept(addr, port);
            gpg::Logf("LOBBY: lan game connection from %s.", conn->ToString().c_str());
            conn->PushReceiver(200, 210, this);
            auto peer = new Moho::SNetPeer{"", -1, addr, port, conn, PS_Pending};
            peer->ListLinkBefore(&this->mPlayers);
        }
    }
    this->mConnector->Pull();
}

// 0x007C8BF0 -> 0x007C56B0
void Moho::CLobby::Pull() {
    if (! this->mReady) {
        return;
    }
    this->mReady = false;

    Moho::CMessage msg{0, LOBMSG_EstablishedPeers};
    Moho::CMessageStream strm{};
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
        if ((*i)->mState == PS_Established) {
            strm.Write<int>((*i)->mUID);
        }
    }
    strm.Write<int>(-1);
    this->BroadcastStream(&msg);
}

inline bool IsInvalidName(Moho::CLobby lobb, const std::string &name, int id) {
    if (lobb->mLocalUID != id && stricmp(lobb->mPlayerName.c_str(), name.c_str()) == 0) {
        return true;
    }
    for (auto i = lobb->mPlayers.begin(); i != lobb->mPlayers.end(); ++i) {
        if ((*i)->mUID != id && stricmp((*i)->mPlayerName.c_str(), name.c_str()) == 0) {
            return true;
        }
    }
    return false;
}
// 0x007C1720
std::string Moho::CLobby::MakeValidPlayerName(std::string &name, int id) {
    constexpr size_t max_len = 24;

    std::string check = name.substr(0, max_len);
    for (int k = 1; IsInvalidName(this, check, id); ++k) {
        std::string num = gpg::STR_Printf("%d", k);
        check = name.substr(0, max_len - num.length()) + num;
    }
    return check;
}

// 0x007C1B20
void Moho::CLobby::HostGame() {
    if (this->mJoinedLobby) {
        throw std::runtime_error{std::string{"Attempting to host or join after already having done so."}};
    }
    this->mJoinedLobby = true;
    if (this->mHasNAT && this->mConnector->GetProtocol() != 0) {
        Moho::INetSocket *sock = Moho::NET_OpenDatagramSocket(15000, this);
        Moho::INetSocket *oldSock = this->mSocket;
        this->mSocket = sock;
        if (oldSock != nullptr) {
            delete(oldSock);
        }
        if (this->mSocket != nullptr) {
            gpg::Logf("LOBBY: Listening for discovery requests on port %d", 15000);
            Moho::WIN_GetWaitHandleSet()->AddHandle(this->mSocket->CreateEvent());
        } else {
            gpg::Logf("LOBBY: Creating discovery listener failed -- someone else must be hosting a game on this machine.");
        }
    }
    if (this->mLocalUID == -1) {
        this->mLocalUID = this->mNextId++;
        gpg::Logf("LOBBY: assigning ourselves the uid of %d", this->mLocalUID);
    }
    this->mTime1 = gpg::time::GetSystemTimer()->ElapsedCycles();
    this->RunScript("Hosting");
}

// 0x007C1DA0
void Moho::CLobby::JoinGame(const char *a, const char *playerName, int playerId) {
    if (this->mJoinedLobby) {
        throw std::runtime_error{std::string{"Attempting to host or join after already having done so."}};
    }
    this->mJoinedLobby = true;
    this->mPeerConnection = this->mConnector->Connect(??, a);
    this->mPeerConnection->PushReceiver(200, 210, a);
    auto peer = new Moho::SNetPeer{
        playerName != nullptr ? playerName : "",
        playerId,
        this->mPeerConnection->GetAddr(),
        this->mPeerConnection->GetPort(),
        this->mPeerConnection,
        Moho::EPeerState::PS_Connecting
    };
    peer->ListLinkBefore(&this->mPlayers);
    gpg::Logf("LOBBY: Connecting to host %s", peer->ToString().c_str());
    LuaPlus::LuaState *state = this->mLuaObj.GetActiveState();
    std::string msg = Moho::Loc(state, "<LOC Engine0001>Connecting to game host...");
    this->RunScript("SystemMessage", msg.c_str());
}

// 0x007C2210
void Moho::CLobby::BroadcastScriptData(const LuaPlus::LuaObject &dat) {
    Moho::CMessage msg{0, LOBMSG_BroadcastScript};
    Moho::CMessageStream strm{&msg};
    if (! Moho::SCR_ToByteStream(dat, strm)) {
        throw std::runtime_error{std::string{"CLobby::BroadcastScriptData(): failed to encode message."}};
    }
    this->BroadcastStream(&msg);
}

// 0x007C24C0
void Moho::CLobby::SendScriptData(int id, const LuaPlus::LuaObject &dat) {
    Moho::CMessage msg{0, LOBMSG_SendScript};
    Moho::CMessageStream strm{&msg};
    if (! Moho::SCR_ToByteStream(dat, strm)) {
        throw std::runtime_error{gpg::Printf("CLobby::SendScriptData(): failed to encode message to UID %d", id)};
    }
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        if ((*i)->mUID == id) {
            (*i)->mPeerConnection->Write(&msg);
        }
    }
    throw std::runtime_error{gpg::Printf("CLobby::SendScriptData(): sending to unknown UID %d?", id)};
}

// 0x007C27E0
LuaPlus::LuaObject Moho::CLobby::GetPeers(LuaPlus::LuaState *state) {
    LuaPlus::LuaObject obj{};
    obj.AssignNewTable(state, 0, 0);
    int k = 1;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        obj.SetObject(k++, (i*)->ToLua(state));
    }
    return obj;
}

// 0x007C38C0
void Moho::CLobby::LaunchGame(const LuaPlus::LuaObject &dat) {
    LuaPlus::LuaObject gameOptions = dat["GameOptions"];
    std::string scenFile{gameOptions["ScenarioFile"].GetString()};
    LuaPlus::LuaObject scenInfo = Moho::WLD_LoadScenarioInfo(scenFile, Moho::USER_GetLuaState());
    if (scenInfo.IsNil()) {
        this->RunScript("LaunchFailed", "");
        return;
    }

    scenInfo.SetObject("Options", gameOptions);
    std::auto_ptr<Moho::LaunchInfoNew> launchInfo = new Moho::LaunchInfoNew{};
    launchInfo->mGameMods = Moho::SCR_ToString(dat["GameMods"]);
    launchInfo->mScenarioInfo = Moho::SCR_ToString(scenInfo);
    launchInfo->mInitSeed = this->mTime1;
    LuaPlus::LuaObject cheats = gameOptions["CheatsEnabled"];
    if (cheats && cheats.IsString() && strcmp(cheats.GetString(), "true") == 0) {
        launchInfo->mCheatsEnabled = true;
    }

    std::vector<std::string> armyNames{};
    LuaPlus::LuaObject teams = scenInfo.Lookup("Configurations.standard.teams");
    if (teams.IsNil()) {
        this->RunScript("LaunchFailed", "NoConfig");
        return;
    }

    for (LuaPlus::LuaTableIterator itr{teams}; itr; ++itr) {
        LuaPlus::LuaObject cur = itr.GetValue();
        if (stricmp("FFA", cur["name"].GetString()) == 0) {
            for (LuaPlus::LuaTableIterator arm{cur["armies"]}; itr; ++itr) {
                armyNames.push_back(std::string{arm.GetValue().GetString()});
            }
            break;
        }
    }
    std::vector<LuaPlus::LuaObject> players{};
    LuaPlus::LuaObject playerOpts = dat["PlayerOptions"];
    if (! playerOpts.IsNil()) {
        for (LuaPlus::LuaTableIterator itr{playerOpts}; itr; ++itr) {
            int ind = itr.GetKey().GetInteger() - 1;
            if (ind >= 0 && ind < armyNames.size() ) {
                itr.GetValue().SetString("ArmyName", armyNames[ind].c_str());
            }
            players.push_back(itr.GetValue());
        }
    }
    if (players.size() > armyNames.size()) {
        this->RunScript("LaunchFailed", "StartSpots");
        return;
    }

    players.shrink_to_fit();
    launchInfo->mCommandSources.mOriginalSource = -1;
    auto obj = new struct_1{};
    obj->mIsNet = this->mConnector->GetProtocol() != NETPROTO_None;
    int numTimeouts = -1;
    if (obj->mIsNet) {
        LuaPlus::LuaObject timeouts = gameOptions["Timeouts"];
        if (timeouts.IsString()) {
            numTimeouts = atoi(timeouts.GetString());
        }
    }
    if (! players.empty()) {
        LuaPlus::LuaObject defPlyr = Moho::RULE_GetDefaultPlayerOptions(Moho::USER_GetLuaState());
        defPlyr.SetString("PlayerName", "default");
        defPlyr.SetString("ArmyName", "default");
        defPlyr.SetBoolean("Human", false);
        players.push_back(defPlyr);
    }

    LuaPlus::LuaObject civAlly = gameOptions["CivilianAlliance"];
    if (civAlly && civAlly.IsString()) {
        const char *ally = civAlly.GetString();
        if (stricmp(ally, "none") != 0) {
            LuaPlus::LuaObject extras = scenInfo.Lookup("Configurations.standard.customprops.ExtraArmies");
            if (extras.IsString()) {
                std::vector<std::string> exVec{};
                gpg::STR_GetTokens(extras.GetString(), " ", exVec);
                for (int k = 0; k < exVec.size(); ++k) {
                    LuaPlus::LuaObject civ = Moho::RULE_GetDefaultPlayerOptions(Moho::USER_GetLuaState());
                    civ.SetString("PlayerName", "civilian");
                    civ.SetString("ArmyName", exVec[k].c_str());
                    civ.SetBoolean("Civilian", true);
                    civ.SetBoolean("Human", false);
                    players.push_back(civ);
                }
            }
        }
    }

    int curSrc = 0;
    int local = -1;
    for (int k = 0; k < players.size(); ++k) {
        Moho::BVIntSet v122{};
        if (players[k].GetBoolean()) {
            int id = atoi(players[k].GetString());
            if (this->mLocalUID == id) {
                launchInfo->mCommandSources.mOriginalSource = k;
            }
            const char *name = players[k]["PlayerName"].GetString();
            this->AssignClientIndex(id, name, curSrc, local);
            int source = this->AssignCommandSource(
                numTimeouts,
                id,
                obj->mSource,
                &launchInfo->mCommandSources
            );
            if (source != 255) {
                v122.Add(source);
            }
        }
        launchInfo.mStrVec.push_back(Moho::SCR_ToString(players[k]));
        launchInfo->mArmyLaunchInfo.push_back(v122);
    }
    LuaPlus::LuaObject obj = dat["Observers"];
    for (LuaPlus::LuaTableIterator itr{obj}; itr; ++itr) {
        LuaPlus::LuaObject cur = itr.GetValue();
        int id = atoi(cur["OwnerID"].GetString());
        this->AssignClientIndex(id, cur["PlayerName"].GetString(), curSrc, local);
        LuaPlus::LuaObject::~LuaObject(&obj);
        this->AssignCommandSource(
            obj->mIsNet ? 0 : - 1,
            id,
            &launchInfo->mCommandSources,
            obj->mSource
        );
    }

    int speed = 0;
    bool adjustableSpeed = false;
    LuaPlus::LuaObject gameSpeed = gameOptions["GameSpeed"];
    if (gameSpeed.IsString()) {
        const char *speedStr = gameSpeed.GetString();
        if (stricmp(speedStr, "fast") == 0) {
            speed = 4;
        } else if (stricmp(speedStr, "adjustable") == 0) {
            adjustableSpeed = true;
        }
    }
    this->mConnector->SelectEvent(NULL);
    Moho::IClientManager *manager = Moho::CLIENT_CreateClientManager(armyNames.size(), this->mConnector, speed, adjustableSpeed);
    Moho::IClientManager *old = obj->mClientManager;
    if (manager != old && old != nullptr) {
        delete(old);
    }
    obj->mClientManager = manager;
    this->mConnector = nullptr;
    obj->mClientManager->CreateLocalClient(
        this->mPlayerName.c_str(),
        players.size(),
        this->mLocalUID,
        obj->mSource
    );
    Moho::SWldSessionInfo *sess = ?;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
        Moho::SNetPeer *cur = *i;
        if (cur->mClientInd == -1) {
            continue;
        }
        if (cur->mState == PS_Established) {
            this->RemoveLinkage(cur->mPeerConnection->Find(200, 210, this));
            this->RemoveLinkage(cur->mPeerConnection->Find(100, 120, this));
            sess->mClientManager->CreateNetClient(
                cur->mPlayerName.c_str(),
                cur->mClientInd,
                cur->mUID,
                cur->mCmdSource,
                cur->mPeerConnection
            );
            cur->mPeerConnection = nullptr;
        } else {
            sess->mClientManager->CreateNullClient(
                cur->mPlayerName.c_str(),
                cur->mClientInd,
                cur->mUID,
                cur->mCmdSource
            );
        }
    }
    while (! this->mPlayers.empty()) {
        Moho::SNetPeer *cur = *this->mPlayer.begin();
        if (cur->mClientInd != -1 && cur->mState != PS_Established) {
            this->mClientManager->GetClient(cur->mClientInd)->Eject();
        }
        if (cur->mPeerConnection != nullptr) {
            cur->mPeerConnection->ScheduleDestroy();
        }
        delete(cur);
    }
    sess->mMapName = ??["map"].GetString();
    sess->mLaunchInfo = boost::shared_ptr{launchInfo};
    this->RunScript("GameLaunched");
    if (sSessionInfo != sess) {
        if (sSessionInfo != nullptr) {
            delete(sSessionInfo);
        }
    }
    sSessionInfo = sess;
    sWldFrameAction = FRAME_ACTION_PRELOAD;
}

// 0x007C4E80
void Moho::CLobby::AssignClientIndex(int id, gpg::StrArg playerName, unsigned int &cur, int &local) {
    if (this->mLocalUID ==id) {
        if (local == -1) {
            local = cur++;
        }
    } else {
        Moho::SNetPeer *peer = nullptr;
        for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
            if ((*i)->mUID == id) {
                peer = *i;
                break;
            }
        }
        if (peer == nullptr) {
            peer = new Moho::SNetPeer{playerName, id, 0, 0, 0, PS_Disconnected};
        }
        if (peer->mClientInd == -1) {
            peer->mClientInd = cur++;
        }
    }
}

// 0x007C4F60
unsigned int Moho::CLobby::AssignCommandSource(int timeouts, int ownerId, std::vector<Moho::SSTICommandSource> &srcs, unsigned int &sourceId) {
    if (this->mLocalUID == ownerId) {
        if (sourceId == 255) {
            sourceId = srcs.size();
            Moho::SSTICommandSource src{sourceId, this->mPlayerName.c_str(), timeouts};
            srcs.push_back(src);
        }
        return sourceId;
    } else {
        Moho::SNetPeer *peer = nullptr;
        for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
            if ((*i)->mUID == ownerId) {
                peer = *i;
                break;
            }
        }
        if (peer->mCmdSource == 255) {
            peer->mCmdSource = srcs.size();
            Moho::SSTICommandSource src{sourceId, this->mPlayerName.c_str(), timeouts};
            srcs.push_back(src);
        }
        return peer->mCmdSource;
    }
}

// 0x007C5B60
void Moho::CLobby::ConnectionFailed(Moho::INetConnection *conn) {
    if (this->mPeerConnection == conn) {
        gpg::Logf("LOBBY: connection to master failed -- giving up.");
        this->RunScript("ConnectionFailed", "HostLeft");
    } else {
        Moho::SNetPeer *peer = nullptr;
        for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
            if ((*i)->mPeerConnection == conn) {
                peer = *i;
                break;
            }
        }
        gpg::Logf("LOBBY: connection to %s failed, retrying...", peer->ToString());
        Moho::INetConnection *conn = this->mConnector->Connect(this->mAddr, this->mPort);
        peer->mPeerConnection = conn;
        conn->PushReceiver(200, 210, this);
        conn->ScheduleDestroy();
    }
}

// 0x007C5CA0
void Moho::CLobby::ConnectionMade(Moho::CMessageStream *strm, Moho::INetConnection *conn) {
    Moho::SNetPeer *peer = nullptr;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        if ((*i)->mPeerConnection == conn) {
            peer = *i;
            break;
        }
    }
    gpg::Logf("LOBBY: connection to %s made, status=%s.", peer->ToString().c_str(), func_PeerStateToString(peer->mState).c_str());
    if (peer->mState == PS_Connecting) {
        peer->mState = PS_Connected;
        conn->PushReceiver(100, 120, this);
        Moho::CMessage msg{0, LOBMSG_ConnectionEstablished};
        Moho::CMessageStream msgStrm{&msg};
        msgStrm.Write(this->mPlayerName);
        msgStrm.Write<int>(this->mLocalUID);
        conn->Write(&msg);
    } else {
        if (peer->mState != PS_Pending) {
            GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 1604, "c:\\work\\rts\\main\\code\\src\\user\\Lobby.cpp");
        }
        peer->mState = PS_WaitingJoin;
        conn->PushReceiver(100, 120, this);
    }
}

// 0x007C5ED0
void Moho::CLobby::Reconnect(Moho::INetConnection *conn) {
    if (this->mPeerConnection == conn) {
        gpg::Logf("LOBBY: host disconnected.");
        this->RunScript("ConnectionFailed", "HostLeft");
        return;
    }
    Moho::SNetPeer *peer = nullptr;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        if ((*i)->mPeerConnection == conn) {
            peer = *i;
            break;
        }
    }
    switch (peer->mState) {
        case PS_Connecting:
        case PS_Connected: {
            gpg::Logf("LOBBY: connection to %s lost, retrying", peer->ToString().c_str());
            Moho::INetConnection retry = this->mConnector->Connect(peer->mAddr, peer->mPort);
            peer->mPeerConnection = retry;
            retry->PushReceiver(200, 210, this);
            peer->mState = PS_Connecting;
            conn->ScheduleDestroy();
            break;
        }
        case PS_Pending: {
            GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 1636, "c:\\work\\rts\\main\\code\\src\\user\\Lobby.cpp");
        }
        case PS_WaitingJoin: {
            gpg::Logf("LOBBY: lost connection to %s, waiting for them to reconnect.", peer->ToString().c_str());
            peer->mState = PS_Pending;
            Moho::INetConnection *reconn = this->mConnector->Accept(peer->mAddr, peer->mPort);
            peer->mPeerConnection = reconn;
            reconn->PushReceiver(200, 210, this);
            conn->ScheduleDestroy();
            break;
        }
        case PS_Established: {
            this->mReady = true;
            if (this->mPeerConnection != nullptr || ! this->mHasNAT) {
                gpg::Logf("LOBBY: lost connection to %s, waiting for them to reconnect.", peer->ToString().c_str());
                this->SystemMessage(
                    Moho::Loc(this->mLuaObj.GetActiveState(), "<LOC Engine0003>Lost connection to %s.").c_str(),
                    peer->mPlayerName.c_str()
                );
                Moho::INetConnection *retry;
                if (peer->mUID < this->mLocalUID && this->mPeerConnection != nullptr) {
                    retry = this->mConnector->Connect(peer->mAddr, peer->mPort);
                    peer->mState = PS_Connecting;
                } else {
                    retry = this->mConnector->Accept(peer->mAddr, peer->mPort);
                    peer->mState = PS_Pending;
                }
                peer->mPeerConnection = retry;
                retry->PushReceiver(200, 210, this);
                conn->ScheduleDestroy();
            } else {
                gpg::Logf("LOBBY: lost connection to %s, ejecting 'em.", peer->ToString().c_str());
                this->SystemMessage(
                    Moho::Loc(this->mLuaObj.GetActiveState(), "<LOC Engine0002>%s disconnected.").c_str(),
                    peer->mPlayerName.c_str()
                );
                peer->mState = PS_Disconnected;
                this->mReady = true;
                this->KickPeer(peer, "Disconnected");
            }
            break;
        }
        default: {
            GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 1690, "c:\\work\\rts\\main\\code\\src\\user\\Lobby.cpp");
        }
    }
}

// 0x007C64C0
void Moho::CLobby::PeerJoined(Moho::CMessage *msg, Moho::INetConnection *conn) {
    Moho::CMessageStream strm{msg};
    gpg::BinaryReader reader{&strm};
    std::string name;
    int id;
    reader.Read(name);
    reader.Read<int>(id);
    Moho::SNetPeer *peer = nullptr;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
        if ((*i)->mPeerConnection == conn) {
            peer = *i;
            break;
        }
    }
    if (peer->mState != PS_WaitingJoin) {
        gpg::Logf("LOBBY: ignoring unexpected join (name=\"%s\", uid=%d) from %s", name.c_str(), id, peer->ToString().c_str());
        return;
    }
    gpg::Logf("LOBBY: Got join (name=\"%s\", uid=%d) from %s", name.c_str(), id, conn->ToString().c_str());
    if (this->mPeerConnection == nullptr) {
        int k = 0;
        for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
            if ((*i)->mState == PS_Established) {
                ++k;
            }
        }
        if (k >= this->mMaxConnections) {
            Moho::CMessage fullMsg{0, LOBMSG_KickPeer};
            Moho::CMessageStream fullStrm{&fullMsg};
            fullStrm.Write("LobbyFull");
            conn->Write(fullMsg);
            conn->ScheduleDestroy();
            delete(peer);
            return;
        }
    }
    peer->mState = PS_Established;
    this->mReady = 1;
    if (peer->mUID == -1) {
        peer->mUID = this->mNextId++;
        gpg::Logf("LOBBY: assigning uid %d", peer->mUID);
    }
    Moho::CMessage joinMsg{0, LOBMSG_PeerJoined};
    if (this->mPeerConnection != nullptr) {
        std::string valName = this->MakeValidPlayerName(name, peer->mUID);
        peer->mPlayerName = valName;
        Moho::CMessageStream joinStrm{&joinMsg};
        joinStrm.Write(this->mPlayerName);
        joinStrm.Write(this->mLocalUID);
        joinStrm.Write(peer->mUID);
        joinStrm.Write(peer->mPlayerName);
        joinStrm.Write(this->mTime1);
    }
    conn->Write(&joinMsg);
    this->SystemMessage(
        Moho::Loc(this->mLuaObj.GetActiveState(), "<LOC Engine0004>Connection to %s established.").c_str(),
        peer->mPlayerName.c_str()
    );
    if (this->mPeerConnection == nullptr && this->mHasNAT) {
        for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
            auto cur = *i;
            if (cur->mState == PS_Established && cur != peer) {
                cur->SendInfoTo(conn);
                peer->SendInfoTo(cur->mPeerConnection);
            }
        }
    }
}

// 0x007C6AD0
void Moho::CLobby::Ejected(Moho::CMessage *msg) {
    Moho::CMessageStream strm{msg};
    gpg::BinaryReader reader{&strm};
    std::string name;
    reader.Read(name);
    this->RunScript("Ejected", name.c_str());
}

// 0x007C6BD0
void Moho::CLobby::Joined(Moho::CMessage *msg, Moho::INetConnection *conn) {
    Moho::SNetPeer *peer = nullptr;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
        if ((*i)->mPeerConnection == conn) {
            peer = *i;
            break;
        }
    }
    if (peer->mState != PS_Connected) {
        gpg::Logf("LOBBY: ignoring unexpected welcome message.");
        return;
    }
    peer->mState = PS_Established;
    this->mReady = true;
    if (this->mPeerConnections == conn) {
        Moho::CMessageStream strm{msg};
        gpg::BinaryReader reader{&strm};
        std::string name;
        int hostid, myId;
        std::string myName;
        int time;
        reader.Read(name);
        reader.Read(hostid);
        reader.Read(myId);
        reader.Read(myName);
        reader.Read(time);
        this->mTime1 = time;
        if (peer->mUID == -1) {
            peer->mPlayerName = name;
            gpg::Logf("LOBBY: welcomed by host \"%s\" (uid=%d)", name.c_str(), hostid);
        }
        if (this->mLocalUID == -1) {
            this->mLocalUID = myId;
            gpg::Logf("LOBBY: assigned uid of %d by host", myId);
        } else if (this->mLocalUID != myId) {
            gpg::Logf("LOBBY: host thinks our uid is %d, but we think it is %d", myId, this->mLocalUID);
        }
        if (this->mPlayerName != myName) {
            gpg::Logf("LOBBY: host renamed us to %s", myName.c_str());
            this->mPlayerName = myName;
        }
        char idStr[16];
        std::to_chars(peer->mUID, idStr);
        char localStr[16];
        std::to_chars(this->mLocalUID, localStr);
        this->RunScript("ConnectionToHostEstablished", localStr, this->mPlayerName.c_str(), idStr);
    }
    this->SystemMessage(
        Moho::Loc(this->mLuaObj.GetActiveState(), "<LOC Engine0004>Connection to %s established.").c_str(),
        peer->mPlayerName.c_str()
    );
}

// 0x007C6EE0
void Moho::CLobby::DataReceived(Moho::CMessage *msg, Moho::INetConnection *conn) {
    Moho::CMessageStream strm{msg};
    LuaPlus::LuaObject obj = Moho::SCR_FromByteStream(&strm, this->mLuaObj.m_state);
    Moho::SNetPeer *peer = nullptr;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        if ((*i)->mPeerConnection == peer) {
            peer = *i;
            break;
        }
    }
    char id[16];
    std::to_chars(peer->mUID, id);
    obj.SetString("SenderID", id);
    obj.SetString("SenderName", peer->mPlayerName.c_str());
    this->RunScript("DataReceived, &obj");
}

// 0x007C7010
void Moho::CLobby::NewPeer(Moho::CMessage *msg, Moho::INetConnection *conn) {
    if (this->mPeerConnection != conn) {    
        gpg::Logf("LOBBY: ignoring NewPeer msg from %s.", conn->ToString().c_str());
        return;
    }
    Moho::CMessageStream strm{msg};
    gpg::BinaryReader reader{&strm};
    std::string name;
    u_long addr;
    u_short port;
    int uid;
    reader.Read(name);
    reader.Read<u_long>(addr);
    reader.Read<u_short>(port);
    reader.Read<int>(uid);
    this->ConnectToPeer(addr, port, name.c_str(), uid);
}

// 0x007C7190
void Moho::CLobby::ConnectToPeer(u_long addr, u_short port, const char *name, int id) {
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
        if ((*i)->mUID == id) {
            throw std::runtime_error{std::string{gpg::STR_Printf("Attempting to redundently add peer uid=%d", uid).c_str()}};
        }
    }
    if (this->mLocalUID == id) {
        throw std::runtime_error{std::string{gpg::STR_Printf("Attempting to add peer uid=%d, but that is us.", id).c_str()}};
    }
    if (! Moho::lob_IgnoreNames.empty()) {
        std::vector<std::string> ignoreNames = func_GetIgnoreNames(func_GetIgnoreNamesSeparators());
        for (auto i = ignoreNames.begin(); i != ignoreNames.end(); ++i) {
            if (*i == name) {
                return;
            }
        }
    }

    Moho::INetConnection *conn;
    Moho::EPeerState state;
    if (id < this->mLocalUID && this->mPeerConnection != nullptr) {
        conn = this->mConnector->Connect(addr, port);
        state = PS_Connecting;
    } else {
        conn = this->mConnector->Accept(addr,port);
        state = PS_Pending;
    }
    conn->PushReceiver(200, 210, this);
    std::string nameStr{name};
    std::string valName = this->MakeValidPlayerName(nameStr, id);
    auto peer = new Moho::SNetPeer{valName.c_str(), id, addr, port, conn, state};
    peer->ListLinkBefore(&this->mPlayers);
    gpg::Logf("LOBBY: Adding peer %s", peer->ToString().c_str());
    this->SystemMessage(
        Moho::Loc(this->mLuaObj.GetActiveState(), "<LOC Engine0005>Connecting to %s...").c_str(),
        this->mPlayerName.c_str()
    );
    return;
}

// 0x007C76A0
void Moho::CLobby::DeletePeer(Moho::CMessage *msg, Moho::INetConnection *conn) {
    if (this->mPeerConnection == conn) {
        Moho::CMessageStream strm{msg};
        gpg::BinaryReader reader{&strm};
        int uid;
        reader.Read(&uid, sizeof(uid));
        gpg::Logf("LOBBY: received LOBMSG_DeletePeer, uid=%d", uid);
        this->DisconnectFromPeer(uid);
    } else {
        gpg::Logf("LOBBY: ignoring DeletePeer msg from %s.", conn->ToString().c_str());
    }
}

// 0x007C7790
void Moho::CLobby::DisconnectFromPeer(int id) {
    if (this->mLocalUID == uid) {
        gpg::Logf("LOBBY: we've been ejected.");
        this->RunScript("Eject", "KickedByHost");
    } else {
        for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
            if ((*i)->mUID == id) {
                this->PeerDisconnected(*i);
                return;
            }
        }
        gpg::Logf("LOBBY: deleting unknown peer uid %d.", uid);
    }
}

// 0x007C77F0
void Moho::CLobby::PeerDisconnected(Moho::SNetPeer *peer) {
    peer->mPeerConnection->ScheduleDestroy();
    peer->ListUnlink();
    char buff[10];
    std::to_chars(peer->mUID, buff);
    this->RunScript("PeerDisconnected", peer->mPlayerName.c_str(), buff);
    if (peer->mState == PS_Established || peer->mState == PS_Disconnected) {
        this->mReady = true;
    }
    if (this->mPeerConnection == nullptr && this->mHasNAT) {
        Moho::CMessage msg{0, LOBMSG_DeletePeer};
        Moho::CMessageStream strm{&msg};
        strm.Write<int>(peer->mUID);
        this->BroadcastStream(&msg);
    }
    delete(peer);
}

// 0x007C7990
void Moho::CLobby::EjectPeer(int id, const char *reason) {
    if (this->mPeerConnection != nullptr) {
        throw std::runtime_error{std::string{"Only the host can eject players."}};
    }
    if (this->mLocalUID == id) {
        throw std::runtime_error{std::string{"We can't eject ourselves!"}};
    }
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        if ((*i)->mUID == id) {
            this->KickPeer(*i, reason);
            return;
        }
    }
    throw std::runtime_error{std::string{"Attempting to eject an unknown peer."}};
}

// 0x007C7AC0
void Moho::CLobby::KickPeer(Moho::SNetPeer *peer, const char *reason) {
    if (this->mPeerConnection != nullptr) {
        throw std::runtime_error{std::string{"Only the host can eject players."}};
    }
    if (peer->mState != PS_Disconnected) {
        Moho::CMessage msg{0, LOBMSG_KickPeer};
        Moho::CMessageStream strm{&msg};
        strm.Write(reason);
        peer->mPeerConnection->Write(&msg);
    }
    this->PeerDisconnected(peer);
}

// 0x007C7C10
void Moho::CLobby::EstablishedPeers(Moho::CMessage *msg, Moho::INetConnection *conn) {
    Moho::SNetPeer *peer = nullptr;
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) { 
        if ((*i)->mPeerConnection == conn) {
            peer = *i;
            break;
        }
    }
    if (peer == nullptr) {
        gpg::Logf("LOBBY: ignoring EstablishedPeers message from unknown connection %s.", conn->ToString().c_str());
        return;
    }

    peer->mConnectedTo.clear();
    Moho::CMessageStream strm{msg};
    gpg::BinaryReader reader{&strm};

    int id;
    reader.Read(id);
    while (id != -1) {
        peer->mConnectedTo.insert(id);
        reader.Read(id);
    }

    std::string builder{};
    for (auto i = this->mConnectedTo.begin(); i != this->mConnectedTo.end(); ++i) {
        if (! builder.empty()) {
            builder.append(", ");
        }
        builder.append(gpg::STR_Printf("%d", *i).c_str());
    }
    gpg::Logf("LOBBY: %s has established connections to: %s", peer->ToString().c_str(), builder.c_str());

    LuaPlus::LuaObject obj{};
    obj.AssignNewTable(this->mLuaObj.GetActiveState(), 0, 0);
    int k = 1;
    char buff[16];
    for (auto i = this->mConnectedTo.begin(); i != this->mConnectedTo.end(); ++i) {
        std::to_chars(*i, buff);
        obj.SetString(k++, buff);
    }
    std::to_chars(peer->mUID, buff);
    this->RunScript("EstablishedPeers", &buff, &obj);
}

// 0x007C7FC0
void Moho::CLobby::SystemMessage(...) {
    std::string str = gpg::STR_Va(va_start, va_arg()); // this is always messy; TODO
    this->RunScript("SystemMessage", str);
}

// 0x007C8040
void Moho::CLobby::BroadcastStream(Moho::CMessage *msg) {
    for (auto i = this->mPlayers.begin(); i != this->mPlayers.end(); ++i) {
        if ((*i)->mState == PS_Established) {
            (*i)->mPeerConnection->Write(msg);
        }
    }
}

// 0x007C0560
std::string func_PeerStateToString(Moho::EPeerState state) {
    if (state >= PS_Count) {
        return gpg::STR_Printf("%d", state);
    } else {
        return std::string{sPeerStateStrings[state]};
    }
}

// 0x007CBC40
std::set<char> func_GetIgnoreNamesSeparators() {
    constexpr const char *sep = ","; // 0x00E16724
    return std::set<char>{sep, &sep[strlen(sep)]};
}

// 0x007CBC80
std::vector<std::string> func_GetIgnoreNames(std::set<char> seps) {
    // This is extremely messy and goes into the byzantine boost function
    // system for some reason. Needless to say, we will be ignoring that
    // and present the effects as follows:
    return Moho::lob_IgnoreNames.split(seps);
}
