#include "gpgcore/containers/fastvector.h"

namespace Moho {

enum EMessageOp : char
{
    MSGOP_Advance = 0,
    MSGOP_SetCommandSource = 1,
    MSGOP_CommandSourceTerminated = 2,
    MSGOP_VerifyChecksum = 3,
    MSGOP_RequestPause = 4,
    MSGOP_Resume = 5,
    MSGOP_SingleStep = 6,
    MSGOP_CreateUnit = 7,
    MSGOP_CreateProp = 8,
    MSGOP_DestroyEntity = 9,
    MSGOP_WarpEntity = 10,
    MSGOP_ProcessInfoPair = 11,
    MSGOP_IssueCommand = 12,
    MSGOP_IssueFactoryCommand = 13,
    MSGOP_IncreaseCommandCount = 14,
    MSGOP_DecreaseCommandCount = 15,
    MSGOP_SetCommandTarget = 16,
    MSGOP_SetCommandType = 17,
    MSGOP_SetCommandCells = 18,
    MSGOP_RemoveCommandFromQueue = 19,
    MSGOP_DebugCommand = 20,
    MSGOP_ExecuteLuaInSim = 21,
    MSGOP_LuaSimCallback = 22,
    MSGOP_EndGame = 23,
    // ...
    MSGOP_Ack = 50,
    MSGOP_Dispatch = 51,
    MSGOP_Available = 52,
    MSGOP_Ready = 53,
    MSGOP_Eject = 54,
    MSGOP_ReceiveChat = 55,
    MSGOP_AdjustSpeed = 56,
    // ...
    MSGOP_ConnectionEstablished = 100,
    MSGOP_KickPeer = 101,
    MSGOP_PeerJoined = 102,
    MSGOP_LobbyMsg1 = 103,
    MSGOP_PeerDisconnected = 104,
    MSGOP_LobbyPull = 105,
    MSGOP_BroadcastScript = 106,
    MSGOP_SendScript = 107,
    // ...
    MSGOP_LobbyJoin = 110,
    MSGOP_LobbyWave = 111,
    // ...
    MSGOP_Msg1 = 200,
    MSGOP_Msg2 = 201,
    MSGOP_Msg3 = 202,
    MSGOP_Msg4 = 203,
    MSGOP_Msg5 = 204,
    MSGOP_Msg6 = 205,
};

struct CMessage
{
    gpg::fastvector_n<char, 64> mBuff;
    int mPos;

    void SetSize(int size) { // inline e.g. 0x0047BE62
        this->mBuff[1] = LOBYTE(size);
        this->mBuff[2] = HIBYTE(size);
    }
    unsigned short GetSize() { // inline e.g. 0x0047BF4C
        // return *(unsigned short *)(&this->mBuff[1]);
        return MAKEWORD(this->mBuff[1], this->mBuff[2]);
    }
    bool HasReadLength() { // inline e.g. 0x0047BEE5
        return this->mPos >= 3;
    }
    Moho::EMessageOp GetType() { // inline e.g. 0x007BFB97
        return (Moho::EMessageOp) this->mBuff[0];
    }
    void SetType(Moho::EMessageOp type) { // inline e.g. 0x004834E9
        this->mBuff[0] = type;
    }
    char *Data() {
        return &this->mBuff[3];
    }

    CMessage() : mBuff{}, mPos{0} {}
    CMessage(int size, Moho::EMessageOp type); // 0x00483490
    bool ReadMessage(gpg::Stream *stream); // 0x0047BD40
    unsigned int Append(char *ptr, int size); // 0x0047BDE0
    int GetMessageSize(); // 0x0047BE90
    bool Read(gpg::Stream *stream); // 0x0047BEE0
};

// 0x00E03BEC
class CMessageStream : public gpg::Stream
{
public:
    struct ReadOnlyTag {};

public: 
    Moho::CMessage *mMsg;

    ~CMessageStream() override = default; // 0x0047C030
    size_t VirtRead(char *buff, size_t len) override; // 0x0047C0F0
    bool VirtAtEnd() override; // 0x0047C120
    void VirtWrite(const char *buff, size_t len) override; // 0x0047C130

    CMessageStream(Moho::CMessage *msg); // 0x0047BFE0
    CMessageStream(Moho::CMessage *msg, ReadOnlyTag); // 0x0047C060
};


}
