#include "core/TDatList.h"
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
    CLIENTMSG_Ack = 50,
    CLIENTMSG_Dispatch = 51,
    CLIENTMSG_Available = 52,
    CLIENTMSG_Ready = 53,
    CLIENTMSG_Eject = 54,
    CLIENTMSG_ReceiveChat = 55,
    CLIENTMSG_AdjustSpeed = 56,
    CLIENTMSG_7 = 57,
    // ...
    LOBMSG_ConnectionEstablished = 100,
    LOBMSG_KickPeer = 101,
    LOBMSG_PeerJoined = 102,
    LOBMSG_NewPeer = 103,
    LOBMSG_DeletePeer = 104,
    LOBMSG_EstablishedPeers = 105,
    LOBMSG_BroadcastScript = 106,
    LOBMSG_SendScript = 107,
    // ...
    DISCOVERMSG_Request = 110,
    DISCOVERMSG_Response = 111,
    // ...
    CONMSG_ConnectionFailed = 200,
    CONMSG_ConnectionMade = 201,
    CONMSG_ConnectionErrored = 202,
    CONMSG_ConnectionQuiet = 203,
    CONMSG_ConnectionTimedOut = 204,
    CONMSG_5 = 205,
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

    template<class T>
    void Write(T val) { // inline
        this->Write((const char *) &val, sizeof(val));
    }
};

struct CMessageDispatcher
{
    Moho::TDatList<Moho::SMsgReceiverLinkage, void> mLinkageList;
    Moho::IMessageReceiver *mReceivers[256];

    void PushReceiver(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec); // 0x0047C360
    void RemoveLinkage(Moho::SMsgReceiverLinkage *); // 0x0047C450
    bool Dispatch(Moho::CMessage *msg); // 0x0047C4D0
    Moho::SMsgReceiverLinkage *Find(unsigned int lower, unsigned int upper, Moho::SMsgReceiverLinkage *recr); // inline
};


// 0x00E03BE4
class IMessageReceiver
{
public:
    Moho::TDatList<Moho::IMessageReceiver, void> mReceiverList;

    virtual void ReceiveMessage(Moho::CMessage *, Moho::CMessageDispatcher *) = 0;

    ~IMessageReceiver(); // 0x0047C4F0
};

struct SMsgReceiverLinkage
{
    Moho::TDatList<Moho::SMsgReceiverLinkage, void> mReceiverLinkages;
    int gap;
    Moho::TDatList<Moho::IMessageReceiver, void> mReceivers;
    unsigned int mLower;
    unsigned int mUpper;
    Moho::IMessageReceiver *mReceiver;
    Moho::CMessageDispatcher *mDispatcher;

    SMsgReceiverLinkage(unsigned int lower, unsigned int upper, Moho::IMessageReceiver *rec, Moho::CMessageDispatcher *disp) :
        mReceiverLinkages{},
        mReceivers{},
        mLower{lower},
        mUpper{upper},
        mReceiver{rec},
        mDispatcher{disp}
    {} // inline 0x0047C37A
};

}
