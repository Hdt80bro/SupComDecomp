// known file

#include "NetUDPConn.h"
#include "core/Thread.h"
#include "gpgcore/General.h"
#include "gpgcore/String.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/thread.hpp"
#include <Windows.h>


// 0x00DFFB68
const char *sPacketStateString[] = {
    "CONNECT",
    "ANSWER",
    "RESETSERIAL",
    "SERIALRESET",
    "DATA",
    "ACK",
    "KEEPALIVE",
    "GOODBYE",
    "NATTRAVERSAL",
};

// 0x00DFFB8C
const char *sUDPStateStrings[] = {
    "Pending",
    "Connecting",
    "Answering",
    "Established",
    "TimedOut",
    "Errored",
};


// 0x0047D0A0
void Moho::struct_a1::Add(int val, LONGLONG time, int size) {
    if ((this->mEnd + 1) % 4096 == this->mStart) {
        this->mStart = (this->mStart + 1) % 4096;
    }
    struct_a2 obj;
    obj.mTime =time;
    obj.v0 = val;
    obj.mSize = size;
    this->Append(&obj);
}

// 0x0047D110
struct_a3 Moho::struct_a1::GetBetween(LONGLONG startTime, LONGLONG endTime) {
    LONGLONG dur = endTime - startTime;
    unsigned int len;
    if (this->mEnd < this->mStart) {
        len = this->mEnd - this->mStart + 4096;
    } else {
        len = this->mEnd - this->mStart;
    }
    unsigned int start = 0;
    unsigned int end = len;
    while (start < end) {
        unsigned int cur = (end + start) / 2;
        if (this->Get(cur).mTime >= dur) {
            end = cur;
        } else {
            start = cur + 1;
        }
    }
    struct_a3 out{dur, end};
    out.mVec.reserve(len - end);
    for (int i = end; i < len; ++i) {
        out.mVec.push_back(this->Get(i));
    }
    return out;
}

// 0x0047D630
void Moho::struct_a1::Append(struct_a2 *obj) {
    struct_a2 *pos = &this->mDat[this->mEnd];
    if (pos != nullptr) {
        *pos = *obj;
    }
    this->mEnd = (this->mEnd + 1) % 4096;
}


// 0x004899E0
Moho::CNetUDPConnector::~CNetUDPConnector() {
    while (this->mPacketList.mNext != &this->mPacketList) {
        if (this->mPacketList.mNext) {
            delete(this->mPacketList.mNext);
        }
    }
    ::closesocket(this->mSocket);
    if (this->mFile != nullptr) { 
        ::fclose(this->mFile);
    }
}

// 0x00489D20
void Moho::CNetUDPConnector::Destroy() {
    boost::shared_ptr<Moho::INetNATTraversalProvider> prov{this->mNatTravProv};
    if (prov) {
        boost::shared_ptr<Moho::INetNATTraversalHandler> nullHandler{};
        int port = this->GetLocalPort();
        prov->SetHandler(port, &nullHandler);
    }
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
        (*i)->ScheduleDestroy();
    }
    this->mClosed = true;
    ::WSASetEvent(this->mEventObject);
}

// 0x00485CA0
Moho::ENetProtocol Moho::CNetUDPConnector::GetProtocol() {
    return Moho::ENetProtocol::NETPROTO_UDP;
}

// 0x0048B250
u_short Moho::CNetUDPConnector::GetLocalPort() {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    sockaddr_in sockaddr;
    int namelen = sizeof(sockaddr);
    ::getsockname(this->mSocket, (SOCKADDR *) &sockaddr, &namelen);
    return ::ntohs(sockaddr.sin_port);
}

// 0x0048B2B0
Moho::INetConnection *Moho::CNetUDPConnector::Connect(u_long addr, u_short port) {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    ::WSASetEvent(this->mEventObject);
    for (auto itr = this->mConnections.begin(); itr != this->mConnections.end(); ++itr) {
        Moho::CNetUDPConnection *cur = *itr;
        if (cur->GetAddr() == addr && cur->GetPort() == port) {
            switch (cur->mState) {
                case Pending: {
                    cur->mState = Answering;
                    return cur;
                }
                case Connecting:
                case Answering:
                case Establishing: {
                    cur->mState = Errored;
                    break;
                }
                case Errored: {
                    break;
                }
                default: GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 2395, "c:\\work\\rts\\main\\code\\src\\core\\NetUDPConn.cpp");
            }
        }
    }
    auto conn = new Moho::CNetUDPConnection{this, addr, port, 1};
    return conn;
}

// 0x0048B410
bool Moho::CNetUDPConnector::FindNextAddr(__out u_long &addr, __out u_short &port) {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    for (auto itr = this->mConnections.begin(); itr != this->mConnections.end(); ++itr) {
        Moho::CNetUDPConnection *conn = *itr;
        if (! conn->mState && ! conn->mScheduleDestroy) { 
            addr = conn->GetAddr();
            port = conn->GetPort();
            return true;            
        }
    }
    return false;
}

// 0x0048B4F0
Moho::INetConnection *Moho::CNetUDPConnector::Accept(u_long addr, u_short port) {
    return this->Connect(addr, port);
}

// 0x0048B500
void Moho::CNetUDPConnector::Reject(u_long addr, u_short port) {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    for (auto itr = this->mConnections.begin(); itr != this->mConnections.end(); ++itr) {
        Moho::CNetUDPConnection *conn = *itr;
        if (conn->GetAddr() == addr && conn->GetPort() == port) { 
            conn->ScheduleDestroy();
        }
    }
}

// 0x0048B5C0
void Moho::CNetUDPConnector::Pull() {
    {
        boost::recursive_mutex::scoped_lock lock{this->mLock};
        this->mIsPulling = true;
        if (! this->mPackets2.empty()) {
            boost::shared_ptr<Moho::INetNATTraversalHandler *> natTravProv{this->mNatTravProv};
            while (! this->mPackets2.empty()) {
                struct_ReceivePacket *rec = *this->mPackets2.begin();
                Moho::SPacket *packet = rec->mPacket;
                u_long addr = rec->mAddr;
                u_short port = rec->mPort;
                this->mPackets2.pop_front();
                if (natTravProv) {
                    lock.unlock();
                    (*natTravProv)->ReceivePacket(
                        addr,
                        port,
                        &packet->mEarlyMask,
                        packet->mSize - 1,
                        this->GetLocalPort() // ??
                    );
                    lock.lock();
                }
                this->DisposePacket(packet);
            }
        }
        for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
            Moho::CNetUDPConnection *conn = *i;
            if (conn->mDestroyed) {
                delete(conn);
            } else {
                conn->FlushInput();
            }
        }
    } // scoped lock end
    for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
        (*i)->DispatchFromInput();
    }
    this->mIsPulling = false;
    if (this->mClosed) {
        ::WSASetEvent(this->mEventObject);
    }
}

// 0x0048B7F0
void Moho::CNetUDPConnector::Push() {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    bool didFlush = false;
    for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
        if ((*i)->FlushOutput()) {
            didFlush = true;
        }
    }
    if (didFlush) {
        ::WSASetEvent(this->mEventObject);
    }
}

// 0x0048B9A0
void Moho::CNetUDPConnector::SelectEvent(HANDLE ev) {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    this->mSelectedEvent = ev;
    ::WSASetEvent(this->mEventObject);
}

// 0x0048B8E0
void Moho::CNetUDPConnector::Debug() {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    gpg::Logf("CNetUDPConnector 0x%08x:", this);
    gpg::Logf("  local port=%d", this->GetLocalPort());
    gpg::Logf("  packet pool size=%d", this->mPacketPoolSize);
    for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
        (*i)->Debug();
    }
}

// 0x0048B9E0
struct_a3 Moho::CNetUDPConnector::Func3(LONGLONG since) {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    return this->mBuff.GetBetween(1000 * since, this->GetTime());
}

// 0x0048BA80
void Moho::CNetUDPConnector::Func1(Moho::CMessage *msg) {
    msg->mBuff.Clear();
    char c = 8;
    msg->mBuff.Append(c);
}

// 0x0048BAE0
void Moho::CNetUDPConnector::ReceivePacket(u_long addr, u_short port, void *dat, size_t size) {
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    if (size > 0x200) {
        gpg::Logf("Truncating NAT traversal packet from %d to %d bytes", size, 512);
        size = 512;
    }
    Moho::SPacket *packet = this->NewPacket();
    memcpy(packet->Data(), dat, size);
    packet->mSize = size;
    Moho::struct_ReceivePacket r{packet, addr, port};
    this->mPackets1.push_back(&r);
    ::WSASetEvent(this->mEventObject);
}


// 0x004896F0
Moho::CNetUDPConnector::CNetUDPConnector(SOCKET sock, boost::weak_ptr<Moho::INetNATTraversalProvider> &prov) :
    Moho::INetConnector{},
    Moho::INetNATTraversalHandler{},
    mLock{},
    mSocket{sock},
    mEventObject{::WSACreateEvent()},
    mNatTravProv{prov},
    mConnections{},
    mPacketList{},
    mPacketPoolSize{0},
    mTimer{},
    mCurTime{0},
    mClosed{false},
    mIsPulling{false},
    mPackets1{},
    mPackets2{},
    mSelectedEvent{0},
    mBuff{},
    mFile{nullptr}
{
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    FILETIME time;
    GetSystemTimeAsFileTime(&time);
    this->mInitTime = (__int64) time / 10;
    ::WSAEventSelect(socket, this->mEventObject, FD_ACCEPT_BIT);
    boost::shared_ptr<Moho::CNetUDPConnector> ptr{this};
    boost::thread thr{boost::function<void>{boost::bind<void>(Moho::CNetUDPConnector::Entry, this, ptr)}};
    lock.unlock();

    boost::shared_ptr<Moho::INetNATTraversalProvider> provider{this->mNatTravProv};
    if (provider) {
        boost::shared_ptr<Moho::INetNATTraversalHandler> handler{ptr};
        provider->SetHandler(this->GetLocalPort(), &handler);
    }
}

// 0x00489E80
Moho::SPacket *Moho::CNetUDPConnector::NewPacket() {
    if (this->mPacketList.ListEmpty()) {
        return new Moho::SPacket{};
    } else {
        --this->mPacketPoolSize;
        Moho::SPacket *packet = this->mPacketList.ListGetNext();
        packet->mList.ListUnlink();
        return packet;
    }
}

// 0x00489ED0
void Moho::CNetUDPConnector::DisposePacket(Moho::SPacket *packet) {
    if (this->mPacketPoolSize >= 20) {
        if (packet != nullptr) {
            delete(packet);
        }
    } else {
        packet->mList.ListLinkBefore(&this->mPacketList);
        ++this->mPacketPoolSize;
    }
}

// 0x00489F30
__int64 Moho::CNetUDPConnector::GetTime() {
    LONGLONG cur = this->mTimer.ElapsedMicroseconds();
    LONGLONG add = this->mInitTime + cur;
    if (add > this->mCurTime) {
        this->mCurTime = add;
    } else {
        ++this->mCurTime;
    }
    return this->mCurTime;
}

// 0x00489F90
void Moho::CNetUDPConnector::Entry(boost::shared_ptr<void>) {
    std::string name = gpg::STR_Printf("CNetUDPConnector for port %d", this->GetLocalPort());
    gpg::SetThreadName(0xFFFFFFFF, name.c_str());
    Moho::THREAD_SetAffinity(0);
    ::SetThreadPriority(GetCurrentThread(), 2);
    boost::recursive_mutex::scoped_lock lock{this->mLock};
    while (true) {
        if (! Moho::net_LogPackets && this->mFile != nullptr) {
            ::fclose(this->mFile);
            this->mFile = nullptr;
        }
        this->ReceiveData();
        LONGLONG timeout = this->SendData();
        if (this->mClosed && ! this->mIsPulling) {
            for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
                if ((*i)->mDestroyed) {
                    delete(*i);
                }
            }
            if (this->mConnections.ListEmpty()) {
                break;
            }
        }
        lock.unlock();

        if (Moho::net_DebugCrash) {
            *(int *) 0 = 0;
        }
        LONGLONG startTime;
        if (Moho::net_DebugLevel >= 3) {
            startTime = this->GetTime();
            std::string timeStr = func_FileTimeToString(startTime);
            gpg::Debugf("%s: waiting, timeout=%dms", timeStr, timeout);
        }
        ::WSAWaitForMultipleEvents(1, &this->mEventObject, false, timeout, true);

        if (startTime != 0) {
            LONGLONG  time = this->GetTime();
            std::string timeStr = func_FileTimeToString(time);
            gpg::Debugf(
                "%s: wait finished, elapsed=%dms",
                timeStr.c_str(),
                (time - startTime) / 1000
            );
        }
        lock.lock()
        ::WSAResetEvent(this->mEventObject);
    }
}

// 0x0048A280
void Moho::CNetUDPConnector::ReceiveData() {
    Moho::TDatList<Moho::SPacket, void> list{};
    while (true) {
        Moho::SPacket *pack = list.ListGetNext();
        if (list.ListEmpty()) {
            pack = this->NewPacket();
            pack->mList.ListLinkBefore(&list);
        }
        Moho::SPacketData *packDat = pack->Data();
        sockaddr_in from;
        int fromlen = sizeof(from);
        int size = ::recvfrom(this->mSocket, packDat, 512, 0, (SOCKADDR *) &from, &fromlen);
        if (size < 0) {
            break;
        }
        pack->mSize = size;

        LONGLONG curTime = this->GetTime();
        pack->mSentTime = curTime;
        this->mBuff.Add(1, curTime, size);
        u_long addr = ::ntohl(from.sin_addr.S_un.S_addr);
        u_short port = ::ntohs(from.sin_port);
        if (Moho::net_LogPackets) {
            func_LogPacket(1, this, pack->mSentTime, addr, port, packDat, size);
        }
        if (Moho::net_DebugLevel >= 2) {
            std::string packStr = pack->ToString();
            std::string hostStr = Moho::NET_GetHostName(addr);
            std::string timeStr = func_FileTimeToString(pack->mSentTime);
            gpg::Debugf("%s:                     recv %s:%d, %s%s",
                    timeStr.c_str(), hostStr.c_str(), port, packStr.c_str(), "");
        }
        if (size > 0 && packDat->mState == NATTRAVERSAL) {
            boost::shared_ptr<Moho::INetNATTraversalProvider> prov{&this->mNatTravProv};
            if (prov) {
                pack->mList.ListUnlink();
                Moho::struct_ReceivePacket obj{pack, addr, port};
                this->mPackets2.push_back(&obj);
                if (this->mSelectedEvent != nullptr) {
                    ::SetEvent(this->mSelectedEvent);
                }
            }
        } else if (size < 15) {
            if (Moho::net_DebugLevel != 0) {
                gpg::Logf("CNetUDPConnector<%d>::ReceiveData(): ignoring short (%d bytes) packet from %s:%d",
                    this->GetLocalPort(),
                    size,
                    Moho::NET_GetHostName(addr).c_str(),
                    port
                );
            }
        } else if (packDat->mState >= Moho::EPacketState::NumStates) {
            if (Moho::net_DebugLevel != 0) {
                gpg::Logf("CNetUDPConnector<%d>::ReceiveData(): ignoring unknown packet type (%d) from %s:%d",
                    this->GetLocalPort(),
                    packDat->mState,
                    Moho::NET_GetHostName(addr).c_str(),
                    port
                );
            }
        } else if (size != pack->mPayloadLength + 15) {
            if (Moho::net_DebugLevel != 0) {
                gpg::Logf(
                    "CNetUDPConnector<%d>::ReceiveData(): ignoring packet with payload length mismatch (got %d, header says %d) from %s:%d",
                    this->GetLocalPort(),
                    size,
                    pack->mPayloadLength + 15,
                    Moho::NET_GetHostName(addr).c_str(),
                    port
                );
            }
        } else if (packDat->mState != CONNECT) {
            bool skip = false;
            for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
                Moho::CNetUDPConnection *conn = *i;
                if (conn->GetAddr() == addr && conn->GetPort() == port && conn->mState < 5 ) {
                    switch (packDat->mState) {
                        case ANSWER: {
                            conn->ProcessAnswer(pack);
                            break;
                        }
                        case DATA: {
                            conn->ProcessData(pack);
                            break;
                        }
                        case ACK: {
                            conn->ProcessAcknowledge(pack);
                            break;
                        }
                        case KEEPALIVE: {
                            conn->ProcessKeepAlive(pack);
                            break;
                        }
                        case GOODBYE: {
                            conn->ProcessGoodbye(pack);
                            break;
                        }
                        default: {
                            if (Moho::net_DebugLevel != 0) {
                                gpg::Logf(
                                    "CNetUDPConnector<%d>::ReceiveData(): ignoring unimplemented packet of type %d from %s:%d",
                                    this->GetLocalPort(),
                                    packDat->mState,
                                    Moho::NET_GetHostName(addr).c_str(),
                                    port
                                );
                            }
                            break;
                        }
                    }
                    skip = true;
                    break;
                }
            }
            if (! skip && Moho::net_DebugLevel != 0) {
                gpg::Logf(
                    "CNetUDPConnector<%d>::ReceiveData(): ignoring packet of type %d from unknown host %s:%d",
                    this->GetLocalPort(),
                    packDat->mState,
                    Moho::NET_GetHostName(addr).c_str(),
                    port
                );
            }
        } else {
            this->ProcessConnect(pack, addr, port);
        }
    }

    if (::WSAGetLastError() != WSAEWOULDBLOCK) {
        const char *err = Moho::NET_GetWinsockErrorString();
        if (Moho::net_DebugLevel) {
            gpg::Logf("CNetUDPConnector<%d>::ReceiveData(): recvfrom() failed: %s", this->GetLocalPort(), err);
        }
    }
    for (auto i = list.begin(); i != list.end(); ++i) {
        this->DisposePacket(*i);
    }
}

// 0x0048AA40
void Moho::CNetUDPConnector::ProcessConnect(Moho::SPacket *pack, u_long addr, u_short port) {
    if (pack->mSize != 60) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf(
                "CNetUDPConnector<%d>::ProcessConnect(): ignoring wrong length CONNECT (got %d bytes, required %d) from %s:%d",
                this->GetLocalPort(),
                pack->mSize,
                60,
                Moho::NET_GetHostName(addr).c_str(),
                port
            );
        }
    } else if (pack->mProtocol != NETPROTO_UDP) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf(
                "CNetUDPConnector<%d>::ProcessConnect(): ignoring connect with wrong protocol (got %d, required %d) from %s:%d",
                this->GetLocalPort(),
                pack->mProtocol,
                NETPROTO_UDP,
                Moho::NET_GetHostName(addr).c_str(),
                port
            );
        }
    } else {
        bool skip = false;
        for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
            Moho::CNetUDPConnection *conn = *i;
            if (conn->GetAddr() == addr
                && conn->GetPort() == port
                && conn->mState < Errored
                && conn->ProcessConnect(pack)
            ) {
                skip = true;
                break;
            }
        }
        if (! skip && ! this->mClosed) {
            auto conn = new Moho::CNetUDPConnection{this, addr, port, 0};
            conn->ProcessConnect(pack);
            if (this->mSelectedEvent != nullptr) {
                ::SetEvent(this->mSelectedEvent);
            }
        }
    }
}

// 0x0048AC40
int Moho::CNetUDPConnector::SendData() {
    int timeout = -1;
    LONGLONG time = this->GetTime();
    while (! this->mPackets1.empty()) {
        struct_ReceivePacket *pack = *this->mPackets1.begin();
        sockaddr_in name;
        name.sin_family = AF_INET;
        name.sin_port = ::htons(pack->mPort);
        name.sin_addr.S_un.S_addr = ::htonl(pack->mAddr);
        ::sendto(this->mSocket, &pack->mPacket->mState, pack->mPacket->mSize, 0, (SOCKADDR *) &name, 16);
        if (Moho::net_LogPackets) {
            func_LogPacket(0, this, time, pack->mAddr, pack->mPort, pack->mPacket->Data(), pack->mPacket->mSize);
        }
        this->mBuff.Add(0, time, pack->mPacket->mSize);
        if (Moho::net_DebugLevel >= 2) {
            std::string packStr = pack->mPacket->ToString();
            std::string hostStr = Moho::NET_GetHostName(pack->mAddr);
            LONGLONG curTime = this->GetTime();
            std::string timeStr = func_FileTimeToString(curTime);
            gpg::Debugf("%s: send %s:%d, %s%s", timeStr.c_str(), hostStr.c_str(), pack->mPort, packStr, "");
        }
        this->mPackets1.pop_front();
        this->DisposePacket(pack->mPacket);
    }
    for (auto i = this->mConnections.begin(); i != this->mConnections.end(); ++i) {
        Moho::CNetUDPConnection *conn = *i;
        int conTimeout = 0;
        while (conTimeout == 0) {
            if (! conn->GetBacklogTimeout(time, conTimeout)) {
                break;
            } else {
                conTimeout = func_ChooseTimeout(conn->SendData(), timeout);
            }
        }
        timeout = func_ChooseTimeout(conTimeout, timeout);
    }
    return timeout;
}

// 0x00485BE0
u_long Moho::CNetUDPConnection::GetAddr() {
    return this->mAddr;
}

// 0x00485BF0
u_short Moho::CNetUDPConnection::GetPort() {
    return this->mPort;
}

// 0x00489550
float Moho::CNetUDPConnection::GetPing() {
    boost::recursive_mutex::scoped_lock lock{this->mConnector->mLock};
    return this->mPingTime;
}

// 0x00489590
float Moho::CNetUDPConnection::GetTime() {
    boost::recursive_mutex::scoped_lock lock{this->mConnector->mLock};
    if (this->mLastRecv != 0) {
        return (this->mConnector->GetTime() - this->mLastRecv) * 0.001;
    } else {
        return -1.0;
    }
}

// 0x00489130
void Moho::CNetUDPConnection::Write(Moho::SDataView *data) {
    boost::recursive_mutex::scoped_lock lock{this->mConnector->mLock};
    if (this->mOutputFilterStream != nullptr) {
        gpg::Stream *out = this->mOutputFilterStream;
        out->Write(data->mStart, data->mEnd - data->mStart);
    } else {
        gpg::Stream *out = &this->mPendingOutputData;
        out->Write(data->mStart, data->mEnd - data->mStart);
    }
    if (this->mSendBy.mTime == 0) {
        LONGLONG v16; // ?
        HIDWORD(v16) = HIDWORD(this->mSendBy.mTime) | LODWORD(this->mSendBy.mTime);
        LODWORD(v16) = 1000;
        this->mSendBy.mTime = Moho::net_SendDelay * v16 + this->mConnector->GetTime();
    }
    this->mHasWritten = true;;
    size_t s = data->mEnd - data->mStart;
    this->mTotalBytesQueued += s;
    this->mTotalBytesQueuedMD5.Update(data->mStart, s);
    if (Moho::net_DebugLevel >= 3) {
        std::string digStr = this->mTotalBytesQueuedMD5.Digest().ToString();
        std::string thisStr = this->ToString();
        std::string timeStr = func_FileTimeToString(this->mConnector->GetTime());
        gpg::Debugf(
        "%s: %s: msg type %d queued [%d bytes, total now %lld, md5 %s]",
            timeStr.c_str(),
            thisStr.c_str(),
            *data->mStart,
            data->mEnd - data->mStart,
            this->mTotalBytesQueued,
            digStr.c_str()
        );
    }
    ::WSASetEvent(this->mConnector->mEventObject);
}

// 0x004893F0
void Moho::CNetUDPConnection::Close() {
    boost::recursive_mutex::scoped_lock lock{this->mConnector->mLock};
    if (! this->mOutputShutdown) {
        gpg::ZLibOutputFilterStream *filter = this->mOutputFilterStream;
        if (filter != nullptr) {
            filter->VirtClose(gpg::Stream::Mode::ModeSend);
            this->mOutputFilterStream = nullptr;
            delete(filter);
        }
        this->mPendingOutputData.VirtClose(gpg::Stream::Mode::ModeSend);
        this->mOutputShutdown = true;
        this->mHasWritten = false;
        ::WSASetEvent(this->mConnector->mEventObject);
    }
}

// 0x004894C0
std::string Moho::CNetUDPConnection::ToString() {
    return gpg::STR_Printf("%s:%d",
            Moho::NET_GetHostName(this->mAddr).c_str(), this->mPort);
}

// 0x00489660
void Moho::CNetUDPConnection::ScheduleDestroy() {
    boost::recursive_mutex::scoped_lock lock{this->mConnector->mLock};
    this->Close();
    this->mScheduleDestroy = true;
    ::WSASetEvent(this->mConnector->mEventObject);
}


// 0x00485D30
Moho::CNetUDPConnection::CNetUDPConnection(Moho::CNetUDPConnector *connector, u_long addr, u_short port, Moho::CNetUDPConnection::State state) :
    mAddr{addr},
    mPort{port},
    mConnector{connector},
    mCompressionMethod{Moho::net_CompressionMethod},
    mCompMet{NETCOMP_None},
    mState{state}
    //...
{
    if (this->mCompressionMethod == NETCOMP_Deflate) {
        gpg::Logf("NET: using deflate compression for sends to %s.", this->ToString().c_str());
        auto filter = new gpg::ZLibOutputFilterStream{&this->mPendingOutputData, 1};
        gpg::ZLibOutputFilterStream *old = this->mOutputFilterStream;
        this->mOutputFilterStream = filter;
        if (old != nullptr) {
            delete(old);
        }
    } else if (this->mCompressionMethod == NETCOMP_None) {
        gpg::Logf("NET: using no compression for sends to %s.", this->ToString().c_str());
    } else {
        gpg::Warnf("Unknown compression method %d, assuming none.", this->mCompressionMethod);
        this->mCompressionMethod = NETCOMP_None;
    }
}

// 0x00486150
Moho::CNetUDPConnection::~CNetUDPConnection() {
    // likely default
}

// 0x00486380
bool Moho::CNetUDPConnection::ProcessConnect(Moho::SPacket *pack) {
    if (pack->mTime <= this->mTime1) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            gpg::Logf("CNetUDPConnection<%d,%s>::ProcessConnect(): ignoring obsolete CONNECT", this->mConnector->GetLocalPort(), thisStr.c_str());
        }
        return true;
    }
    this->mLastRecv = pack->mSentTime;
    if (pack->mCompMethod >= Moho::ENetCompressionMethod::NETCOMP_Count) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            int port = this->mConnector->GetLocalPort();
            gpg::Logf(
                "CNetUDPConnection<%d,%s>::ProcessConnect(): ignoring connect w/ unknown compression method (%d)",
                port,
                thisStr.c_str(),
                pack->mCompMethod
            );
        }
        return true;
    }
    switch (this->mState) {
        case Pending:
        case Answering: {
            qmemcpy(this->mDat2, pack->mDat1, sizeof(this->mDat2));
            this->mTime1 = pack->mTime;
            this->mCompMet = pack->mCompMethod;
            this->ReceiveTime(pack);
            return true;
        }
        case Connecting: {
            qmemcpy(this->mDat2, pack->mDat1, sizeof(this->mDat2));
            this->mTime1 = pack->mTime;
            this->mCompMet = pack->mCompMethod;
            this->ReceiveTime(pack);
            this->mState = Answering;
            return true;
        }
        case Establishing:
        case TimedOut: {
            this->mState = Errored;
            this->mReceivedEndOfInput = true;
            this->mInputBuffer.VirtClose(gpg::Stream::Mode::ModeSend);
            if (this->mConnector->mSelectedEvent != nullptr) {
                ::SetEvent(this->mConnector->mSelectedEvent);
            }
            return false;
        }
        default: GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 884, "c:\\work\\rts\\main\\code\\src\\core\\NetUDPConn.cpp");
    }
}

// 0x004865E0
void Moho::CNetUDPConnection::ProcessAnswer(Moho::SPacket *pack) {
    if (pack->mSize != 92) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            u_short port = this->mConnector->GetLocalPort();
            gpg::Logf(
                "CNetUDPConnection<%d,%s>::ProcessAnswer(): ignoring wrong length ANSWER (got %d bytes, required %d)",
                port,
                thisStr.c_str(),
                pack->mSize,
                92
            );
        }
        return;
    }
    if (pack->mTime <= this->mTime1) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            u_short port = this->mConnector->GetLocalPort();
            gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAnswer(): ignoring obsolete ANSWER", port, thisStr.c_str());
        }
        return;
    }
    if (/*or similar*/strncmp(this->mDat1, pack->mDat2, 32) != 0) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            u_short port = this->mConnector->GetLocalPort();
            gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAnswer(): ignoring ANSWER with wrong receiver nonce", port, thisStr.c_str());
        }
        return;
    }
    if (pack->mCompMethod >= Moho::ENetCompressionMethod::NETCOMP_Count) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            u_short port = this->mConnector->GetLocalPort();
            gpg::Logf(
                "CNetUDPConnection<%d,%s>::ProcessConnect(): ignoring answer w/ unknown compression method (%d)",
                port,
                thisStr.c_str(),
                pack->mCompMethod
            );
        }
        return;
    }
    if (this->mState != 0) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            u_short port = this->mConnector->GetLocalPort();
            gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAnswer(): ignoring ANSWER on pending connection", port, thisStr.c_str());
        }
        return;
    }
    if (this->mState != 1 && this->mState != 2) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            u_short port = this->mConnector->GetLocalPort();
            gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAnswer(): ignoring ANSWER on established connection", port, thisStr.c_str());
        }
        return;
    }
    this->RecordPacket(pack);
    this->mLastRecv = pack->mSentTime;
    qmemcpy(this->mDat2, pack->mDat1, sizeof(this->mDat2));
    this->mTime1 = pack->mTime;
    this->mCompMet = pack->mCompMethod;
    this->CreateFilterStream();
    this->ReceiveTime(pack);
    if (this->mConnector->mSelectedEvent != nullptr) {
        ::SetEvent(this->mConnector->mSelectedEvent);
    }
}

// 0x00486910
void Moho::CNetUDPConnection::CreateFilterStream() {
    this->mState = Establishing;
    Moho::CMessage msg{0, MSGOP_Msg2};
    int len = msg.mBuff.Size();
    this->mInputBuffer.Write(msg.mBuff.begin(), len);
    if (this->mCompMet == Moho::ENetCompressionMethod::NETCOMP_None) {
        gpg::Logf("NET: using no compression for receives from %s", this->ToString().c_str());
    } else if (this->mCompMet == Moho::ENetCompressionMethod::NETCOMP_Deflate) {
        gpg::Logf("NET: using deflate compression for receives from %s", this->ToString().c_str());
        auto filter = new gpg::ZLibOutputFilterStream{&this->mInputBuffer, gpg::ZLibOutputFilterStream::OP_Deflate};
        gpg::ZLibOutputFilterStream *old = this->mFilterStream;
        this->mFilterStream = filter;
        delete(old);
    } else {
        GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 982, "c:\\work\\rts\\main\\code\\src\\core\\NetUDPConn.cpp");
    }
}

// 0x00486B10
bool Moho::CNetUDPConnection::ProcessAck(Moho::SPacket *pack) {
    switch (this->mState) {
        case Pending: {
            if (Moho::net_DebugLevel != 0) {
                std::string thisStr = this->ToString();
                int port = this->mConnector->GetLocalPort();
                gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAck(): ignoring traffic on Pending connection.", port, thisStr.c_str());
            }
            return false;
        }
        case Connecting: {
            if (Moho::net_DebugLevel != 0) {
                std::string thisStr = this->ToString();
                int port = this->mConnector->GetLocalPort();
                gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAck(): ignoring traffic on Connecting connection.", port, thisStr.c_str());
            }
            return false;
        }
        case Answering: {
            this->CreateFilterStream();
            break;
        }
        case Establishing: {
            break;
        }
        case TimedOut: {
            this->mState = Establishing;
            break;
        }
        case Errored: {
            if (Moho::net_DebugLevel != 0) {
                std::string thisStr = this->ToString();
                int port = this->mConnector->GetLocalPort();
                gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAck(): ignoring traffic on Errored connection.", port, thisStr.c_str());
            }
            return false;
        }
        default: GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 1024, "c:\\work\\rts\\main\\code\\src\\core\\NetUDPConn.cpp");
    }
    
    if (this->mNextSequenceNumber - pack->mExpectedSequenceNumber < 0) {
        if (Moho::net_DebugLevel != 0) {
            std::string thisStr = this->ToString();
            int port = this->mConnector->GetLocalPort();
            gpg::Logf(
                "CNetUDPConnection<%d,%s>::ProcessHeader(): ignoring acknowledgement of data we haven't sent (packet=%d, next=%d)",
                port,
                thisStr.c_str(),
                pack->mExpectedSequenceNumber,
                this->mNextSequenceNumber
            );
        }
        return false;
    }
    this->RecordPacket(pack);
    for (auto i = this->mUnackedPayloads.begin(); i != this->mUnackedPayloads.end(); ++i) {
        Moho::SPacket *cur = *i;
        short delta = cur->mSequenceNumber - pack->mExpectedSequenceNumber;
        if (delta < 0 || delta - 1 <= 0x1F && ((1 << (delta - 1)) & pack->mEarlyMask) != 0) {
            this->mConnector->DisposePacket(cur);
        }
    }
    if (pack->mExpectedSequenceNumber - this->mRemoteExpectedSequenceNumber > 0) {
        this->mRemoteExpectedSequenceNumber = pack->mExpectedSequenceNumber;
    }
    return true;
}

// 0x00486DB0
void Moho::CNetUDPConnection::ProcessData(Moho::SPacket *pack) {
    if (this->ProcessAck(pack)) {
        this->ReceiveTime(pack);
        int delta = pack->mSequenceNumber - this->mExpectedSequenceNumber;
        if (delta < 0) {
            if (Moho::net_DebugLevel != 0) {
                std::string thisStr = this->ToString();
                int port = this->mConnector->GetLocalPort();
                gpg::Logf(
                    "CNetUDPConnection<%d,%s>::ProcessData(): ignoring repeat of old DATA (seqno=%d, expected=%d)",
                    port,
                    thisStr.c_str(),
                    pack->mSequenceNumber,
                    this->mExpectedSequenceNumber
                );
            }
        } else if (delta > 32) {
            if (Moho::net_DebugLevel != 0) {
                std::string thisStr = this->ToString();
                int port = this->mConnector->GetLocalPort();
                gpg::Logf(
                    "CNetUDPConnection<%d,%s>::ProcessData(): ignoring DATA from too far in the furture (seqno=%d, expected=%d, delta=%d)",
                    port,
                    thisStr.c_str(),
                    pack->mSequenceNumber,
                    this->mExpectedSequenceNumber,
                    delta
                );
            }
        } else {
            for (auto i = this->mEarlyPackets.begin(); i != this->mEarlyPackets.end(); ++i) {
                int seqNum = (*i)->mSequenceNumber;
                if (seqNum == pack->mSequenceNumber) {
                    if (Moho::net_DebugLevel != 0) {
                        std::string thisStr = this->ToString();
                        int port = this->mConnector->GetLocalPort();
                        gpg::Logf(
                            "CNetUDPConnection<%d,%s>::ProcessData(): ignoring repeat of future DATA (seqno=%d, expected=%d)",
                            port,
                            thisStr.c_str(),
                            pack->mSequenceNumber,
                            this->mExpectedSequenceNumber
                        );
                    }
                    return;
                }
                if (pack->mSequenceNumber - seqNum < 0) {
                    break;
                }
            }
            pack->mList.ListLinkBefore(&this->mEarlyPackets);
            this->mLastRecv = pack->mSentTime;
            bool select = false;
            while (! this->mEarlyPackets.ListEmpty()) {
                Moho::SPacket *cur = *this->mEarlyPackets.begin();
                if (this->mReceivedEndOfInput) {
                    if (Moho::net_DebugLevel != 0) {
                        std::string thisStr = this->ToString();
                        int port = this->mConnector->GetLocalPort();
                        gpg::Logf("CNetUDPConnection<%d,%s>::ProcessAnswer(): ignoring DATA after end-of-stream", port, thisStr.c_str());
                    }
                    this->mConnector->DisposePacket(cur);
                } else {
                    if (cur->mSequenceNumber != this->mExpectedSequenceNumber) {
                        break;
                    }
                    if (cur->mPayloadLength != 0) {
                        size_t len = cur->mPayloadLength;
                        this->mTotalBytesReceived += len;
                        char *dat = &cur->mProtocol;
                        this->mTotalBytesReceivedMD5.Update(dat, len);
                        if (this->mFilterStream != nullptr) {
                            this->mFilterStream->Write(dat, len);
                        } else {
                            this->mInputBuffer.Write(dat, len);
                        }
                    } else {
                        if (this->mFilterStream != nullptr) {
                            this->mFilterStream->VirtClose(gpg::Stream::Mode::ModeSend);
                            gpg::ZLibOutputFilterStream *old = this->mFilterStream;
                            this->mFilterStream = nullptr;
                            delete(old);
                        }
                        this->mInputBuffer.VirtClose(gpg::Stream::Mode::ModeSend);
                        this->mReceivedEndOfInput = true;
                    }
                    ++this->mExpectedSequenceNumber;
                    this->mConnector->DisposePacket(cur);
                    select = true;
                }
            }
            this->mMask = 0;
            for (auto i = this->mEarlyPackets.begin(); i != this->mEarlyPackets.end(); ++i) {
                this->mMask |= 1 << ((*i)->mSequenceNumber - this->mExpectedSequenceNumber - 1);
            }
            if (select && this->mConnector->mSelectedEvent != nullptr) {
                ::SetEvent(this->mConnector->mSelectedEvent);
            }
        }
    }
}

// 0x00487310
void Moho::CNetUDPConnection::ProcessAcknowledge(Moho::SPacket *pack) {
    if (this->ProcessAck(pack)) {
        this->mLastRecv = pack->mSentTime;
    }
}

// 0x00487340
void Moho::CNetUDPConnection::ProcessKeepAlive(Moho::SPacket *pack) {
    if (this->ProcessAck(pack)) {
        this->mLastRecv = pack->mSentTime;
        this->ReceiveTime(pack);
    }
}

// 0x00487370
void Moho::CNetUDPConnection::ProcessGoodbye(Moho::SPacket *pack) {
    if (this->ProcessAck(pack)) {
        this->mLastRecv = pack->mSentTime;
        if (! this->mReceivedEndOfInput) {
            this->mState = Errored;
            this->mReceivedEndOfInput = true;
            if (this->mFilterStream != nullptr) {
                this->mFilterStream->VirtClose(gpg::Stream::Mode::ModeSend);
                gpg::ZLibOutputFilterStream *old = this->mFilterStream;
                this->mFilterStream = nullptr;
                delete(old);
            }
            this->mInputBuffer.VirtClose(gpg::Stream::Mode::ModeSend);
            if (this->mConnector->mSelectedEvent != nullptr) {
                ::SetEvent(this->mConnector->mSelectedEvent);
            }
        }
    }
}

// 0x004874C0
void Moho::CNetUDPConnection::RecordPacket(Moho::SPacket *pack) {
    if (pack->mInResponseTo != 0) {
        int ind = pack->mInResponseTo & 0x7F;
        if (this->mTimings[ind].mSource == mInResponseTo) {
            int raw = this->mTimings[ind].mTime.ElapsedMilliseconds();
            this->mPings.roll(raw);
            int filtered = this->mPings.median();
            this->mPingTime = filtered;
            int jitter = this->mPings.jitter(filtered);
            if (Moho::net_DebugLevel != 0) {
                gpg::Logf("Ping time: raw=%7.3fms filtered=%7.3fms, jitter=%7.3f", raw, filtered, jitter);
            }
            this->mKeepAliveFreq = 2'000'000;
        }
    }
}

// 0x00487590
void Moho::CNetUDPConnection::FlushInput() {
    if (! this->mScheduleDestroy && ! this->mReceivedEndOfInput) {
        if (this->mFilterStream != nullptr) {
            this->mFilterStream->VirtFlush();
        }
        this->mInputBuffer.VirtFlush();
    }
}

// 0x004876A0
void Moho::CNetUDPConnection::DispatchFromInput() {
    if (this->mDispatchedEndOfInput || this->mScheduleDestroy) {
        return;
    }
    while (this->mMessage.Read(&this->mInputBuffer)) {
        if (this->mMessage.GetType() != MSGOP_Msg2) {
            size_t len = this->mMessage.mBuff.Size();
            this->mTotalBytesDispatched += len;
            this->mTotalBytesDispatchedMD5.Update(&this->mMessage.mBuff[0], len);
        }
        if (Moho::net_DebugLevel >= 3) {
            std::string digStr = this->mTotalBytesDispatchedMD5.Digest().ToString();
            std::string thisStr = this->ToString();
            std::string timeStr = func_FileTimeToString(this->mConnector->GetTime());
            gpg::Debugf(
                "%s: %s: msg type %d dispatched [%d bytes, total now %lld, md5 %s]",
                timeStr.c_str(),
                thisStr.c_str(),
                &this->mMessage.mBuff[0],
                this->mMessage.mBuff.Size(),
                this->mTotalBytesDispatched,
                digStr.c_str()
            );
        }
        Moho::IMessageReceiver *recr = this->mReceivers[this->mMessage.GetType()];
        if (recr == nullptr) {
            std::string host = Moho::NET_GetHostName(this->mAddr);
            gpg::Warnf(
                "No receiver for message type %d received from %s:%d.",
                this->mMessage.GetType(),
                host.c_str(),
                this->mPort
            );
        } else {
            recr->Receive(&this->mMessage, this);
        }
        this->mMessage.mBuff.Clear();
        this->mMessage.mPos = 0;
        if (this->mScheduleDestroy) {
            return;
        }
    }
    if (this->mInputBuffer.LeftToRead() == 0 && this->mInputBuffer.VirtAtEnd()) {
        this->mDispatchedEndOfInput = true;
        if (this->mState == 5) {
            this->Dispatch(&Moho::CMessage{0, MSGOP_Msg3});
        } else {
            this->Dispatch(&Moho::CMessage{0, MSGOP_Msg4});
        }
    }
}

// 0x004879E0
bool Moho::CNetUDPConnection::FlushOutput() {
    if (! this->mOutputShutdown) {
        if (this->mOutputFilterStream != nullptr) {
            this->mOutputFilterStream->VirtFlush();
        }
        this->mPendingOutputData.VirtFlush();
        this->mFlushedOutputData = this->mPendingOutputData.GetLength();
    }
    return this->mFlushedOutputData != 0;
}

// 0x00487B90
void Moho::CNetUDPConnection::Debug() {
    LONGLONG time = this->mConnector->GetTime();
    gpg::Logf("  CNetUDPConnection 0x%08x:", this);
    std::string addr = Moho::NET_GetDottedOctetFromUInt32(this->mAddr);
    std::string hostname = Moho::NET_GetHostName(this->mAddr);
    gpg::Logf("    remote addr: %s[%s]:%d", hostname.c_str(), addr.c_str(), this->mPort);
    const char *state;
    if (this->mState >= 6) {
        state = gpg::STR_Printf("??? (%d)", this->mState).c_str();
    } else {
        state = sUDPStateStrings[this->mState];
    }
    gpg::Logf("    State: %s", state);
    gpg::Logf("    Last Send: %7dusec ago", time - this->mLastSend.mTime);
    gpg::Logf("    Last Recv: %7dusec ago", time - this->mLastRecv);
    gpg::Logf("    Last KeepAlive: %7dusec ago", time - this->mLastKeepAlive);
    gpg::Logf("    KeepAlive Freq: %d", this->mKeepAliveFreq);
    gpg::Logf("    Next Serial Number: %d", this->mNextSerialNumber);
    gpg::Logf("    In Response To: %d", this->mInResponseTo);
    if (this->mSendBy.mTime != 0) {
        gpg::Logf("    Send By: %7dusec from now", this->mSendBy.mTime - time);
    } else {
        gpg::Logf("    Send By: NA");
    }
    LONGLONG sentTime = this->GetBacklog(time);
    gpg::Logf("    Backlog: %d bytes", sentTime);
    gpg::Logf("    Next Sequence Number: %d", this->mNextSequenceNumber);
    gpg::Logf("    Remote Expected Sequence Number: %d", this->mRemoteExpectedSequenceNumber);
    gpg::Logf("    Expected Sequence Number: %d", this->mExpectedSequenceNumber);
    gpg::Logf("    Pending Output Data: %d", this->mPendingOutputData.GetLength());
    gpg::Logf("    Flushed Output Data: %d", this->mFlushedOutputData);
    gpg::Logf("    Output Shutdown: %s", this->mOutputShutdown ? "true" : "false");
    gpg::Logf("    Sent Shutdown: %s", this->mSentShutdown ? "true" : "false");
    gpg::Logf("    Unacked Payloads:");
    for (auto i = this->mUnackedPayloads.begin(); i != this->mUnackedPayloads.end(); ++i) {
        func_UDPPacketLog("Sent", *i, time);
    }
    gpg::Logf("    Ping Time=%f", this->mPingTime);
    gpg::Logf("    Early Packets:");
    for (auto i = this->mEarlyPackets.begin(); i != this->mEarlyPackets.end(); ++i) {
        func_UDPPacketLog("Received", *i, time);
    }
    gpg::Logf("    Buffered Input Data: %d bytes", this->mInputBuffer.GetLength());
    gpg::Logf("    Received End of Input: %s", this->mReceivedEndOfInput ? "true" : "false");
    gpg::Logf("    Dispatched End of Input: %s", this->mDispatchedEndOfInput ? "true" : "false");
    gpg::Logf("    Closed: %s", this->mScheduleDestroy);
    gpg::Logf("    Total bytes queued: %llu [%s]", this->mTotalBytesQueued, this->mTotalBytesQueuedMD5.Digest().ToString().c_str());
    gpg::Logf("    Total bytes sent: %llu [%s]", this->mTotalBytesSent, this->mTotalBytesSentMD5.Digest().ToString().c_str());
    gpg::Logf("    Total bytes received: %llu [%s]", this->mTotalBytesReceived, this->mTotalBytesReceivedMD5.Digest().ToString().c_str());
    gpg::Logf("    Total bytes dispatched: %llu [%s]", this->mTotalBytesDispatched, this->mTotalBytesDispatchedMD5.Digest().ToString().c_str());
}

// 0x00488150
int func_ChooseTimeout(int old, int choice) {
    if (old == -1 || choice != -1 && choice < old) {
        return choice;
    }
    return old;
}

// 0x00488170
__int64 Moho::CNetUDPConnection::CalcResendDelay(Moho::SPacket *packet) {
    float resendTime = this->mPingTime * Moho::net_ResendPingMultiplier + Moho::net_ResendDelayBias;
    __int8 resendCount = packet->mResendCount;
    if (resendCount >= 3) {
        resendCount = 3;
    }
    int resendDelay = (1 << resendCount) * resendTime;
    if (resendDelay >= Moho::net_MaxResendDelay) {
        resendDelay = Moho::net_MaxResendDelay;
    }
    if (resendDelay < Moho::net_MinResendDelay) {
        resendDelay = Moho::net_MinResendDelay;
    }
    return 1000 * resendDelay;
}

// 0x004881F0
int Moho::CNetUDPConnection::TimeSince(LONGLONG time) {
    int since = (time - this->mConnector->GetTime()) / 1000;
    if (since < 0) {
        return 0;
    }
    return since;
}

// 0x00488220
void Moho::CNetUDPConnection::ReceiveTime(Moho::SPacket *pack) {
    int v3 = HIDWORD(this->mSendBy.mTime) | LODWORD(this->mSendBy.mTime);
    this->mInResponseTo = pack->mSerialNumber;
    if (this->mSendBy.mTime == 0) {
        __int64 v4;
        HIDWORD(v4) = v3;
        LODWORD(v4) = 1000;
        this->mSendBy.mTime = pack->mSentTime + Moho::net_AckDelay * v4;
    }
}

// 0x00488260
int Moho::CNetUDPConnection::GetBacklog(__int64 time) {
    if (this->mSendTime != 0) {
        int result = this->mSendTime - (time - this->mLastSend.mTime) * Moho::net_MaxSendRate * 0.000001;
        if (result > 0) {
            return result;
        }
        this->mSendTime = 0;
    }
    return 0;
}

// 0x004882C0
bool Moho::CNetUDPConnection::GetBacklogTimeout(LONGLONG time, __out int timeout&) { 
    int bl = this->GetBacklog(time);
    if (bl > Moho::net_MaxBacklog) {
        timeout = 1000 * (bl - Moho::net_MaxBacklog) / Moho::net_MaxSendRate;
        return false;
    } else {
        timeout = 0;
        return true;
    }
}

// 0x00488300
int Moho::CNetUDPConnection::SendData() {
    if (this->mDestroyed) {
        return -1;
    }
    switch (this->mState) {
        case Pending:
        case Errored: {
            if (this->mScheduleDestroy) {
                this->mDestroyed = true;
                return -1;
            }
            return -1;
        }
        case Connecting: {
            if (this->mScheduleDestroy) {
                this->mDestroyed = true;
                return -1;
            }
            if (this->mInResponseTo != 0 || this->mConnector->GetTime() - this->mLastSend.mTime > 1'000'000) {
                this->SendPacket(this->NewConnectionPacket());
            }
            return this->TimeSince(this->mLastSend.mTime + 1'000'000);
        }
        case Answering: {
            if (this->mScheduleDestroy) {
                this->mDestroyed = true;
                return -1;
            }
            if (this->mInResponseTo != 0 || this->mConnector->GetTime() - this->mLastSend.mTime > 1'000'000) {
                this->SendPacket(this->NewAnswerPacket());
            }
            return this->TimeSince(this->mLastSend.mTime + 1'000'000);
        }
        case Establishing:
            LONGLONG curTime = this->mConnector->GetTime();
            if (curTime - this->mLastRecv > 10'000'000) {
                if (Moho::net_DebugLevel != 0) {
                    std::string thisStr = this->ToString();
                    int port = this->mConnector->GetLocalPort();
                    gpg::Logf("CNetUDPConnection<%d,%s>::SendData(): connection timed out.", port, thisStr.c_str());
                }
                this->mState = TimedOut;
                __fallthrough; // the fallthrough point *is* state 4, so this likely works as intended
            } else {
                Moho::SPacket *packet;
                if (! this->mUnackedPayloads.ListEmpty() && curTime >= this->mUnackedPayloads.ListGetNext()->mSentTime) {
                    packet = this->mUnackedPayloads.ListGetNext();
                    packet->mList.ListUnlink();
                } else if (this->HasPacketWaiting(curTime)) {
                    packet = this->ReadPacket();
                } else if (this->mScheduleDestroy && this->mSentShutdown && this->mUnackedPayloads.ListEmpty()) {
                    packet = this->NewGoodbyePacket();
                    this->mDestroyed = true;
                } else if (curTime >= this->mLastKeepAlive + this->mKeepAliveFreq) {
                    packet = this->NewPacket(6, 0, true);
                } else if (this->mInResponseTo == 0 && curTime >= this->mSendBy.mTime) {
                    packet = this->NewPacket(5, 0, true);
                }
                if (packet != nullptr) {
                    this->SendPacket(packet);
                }

                if (this->mFlushedOutputData || this->mPendingOutputData.GetLength() > 0x1F1) {
                    return 0;
                }
                LONGLONG since = this->TimeSince(this->mLastKeepAlive + this->mKeepAliveFreq);
                if (! this->mUnackedPayloads.ListEmpty()) {
                    LONGLONG unkTime = this->mUnackedPayloads.ListGetNext()->mSentTime;
                    since = func_ChooseTimeout(since, this->TimeSince(unkTime));
                }
                if (this->mSendBy.mTime != 0 &&
                    (this->mInResponseTo != 0 || this->mPendingOutputData.GetLength() != 0 || this->mHasWritten)
                ) {
                    since = func_ChooseTimeout(since, this->TimeSince(this->mSendBy.mTime));
                }
                return since;
            }
        case TimedOut: {
            if (this->mScheduleDestroy) {
                this->mDestroyed = true;
                return -1;
            }
            if (this->mConnector->GetTime() - this->mLastKeepAlive > this->mKeepAliveFreq) {
                this->SendPacket(this->NewPacket(6, 0, true));
            }
            return this->TimeSince(this->mLastKeepAlive + this->mKeepAliveFreq);
        }
        default: GPG_UNREACHABLE(); // gpg::HandleAssertFailure("Reached the supposably unreachable.", 1562, "c:\\work\\rts\\main\\code\\src\\core\\NetUDPConn.cpp");
    }
}

// 0x00488730
bool Moho::CNetUDPConnection::HasPacketWaiting(__int64 time) {
    if (this->mNextSequenceNumber - this->mRemoteExpectedSequenceNumber > 32) {
        return false;
    }
    if (this->mPendingOutputData.LeftToRead() == 0
        && this->mPendingOutputData.VirtAtEnd()
        && !this->mSentShutdown
    ) {
        return true;
    }
    if (time <= this->mSendBy.mTime && ! this->mFlushedOutputData && time < this->mLastKeepAlive + this->mKeepAliveFreq) {
        return this->mPendingOutputData.GetLength() >= 0x1F1;
    }
    if (this->mHasWritten) {
        if (this->mOutputFilterStream != nullptr) {
            this->mOutputFilterStream->VirtFlush();
        }
        this->mPendingOutputData.VirtFlush();
        this->mHasWritten = false;
    }
    return this->mPendingOutputData.GetLength() != 0;
}

// 0x00488810
Moho::SPacket *Moho::CNetUDPConnection::NewConnectionPacket() {
    Moho::SPacket *pack = this->NextPacket(CONNECT, 45, false);
    pack->mProtocol = NETPROTO_UDP;
    pack->mTime = this->mConnector->GetTime();
    pack->mCompMethod = this->mCompressionMethod;
    qmemcpy(pack->mDat1, this->mDat1, sizeof(pack->mDat1));
    return pack;
}

// 0x004888C0
Moho::SPacket *Moho::CNetUDPConnection::NewAnswerPacket() {
    Moho::SPacket *pack = this->NewPacket(ANSWER, 77, false);
    pack->mProtocol = NETPROTO_UDP;
    pack->mTime = this->mConnector->GetTime();
    pack->mCompMethod = this->mCompressionMethod;
    qmemcpy(pack->mDat1, this->mDat1, sizeof(pack->mDat1));
    qmemcpy(pack->mDat2, this->mDat2, sizeof(pack->mDat2));
    return pack;
}

// 0x00488980
Moho::SPacket *Moho::CNetUDPConnection::ReadPacket() {
    size_t length = this->mPendingOutputData.GetLength();
    if (length >= 497) {
        length = 0x1F1;
    }
    Moho::SPacket *pack = this->NewPacket(DATA, length, true);
    if (length > 0) {
        this->mPendingOutputData.Write(&pack->mProtocol, length);
        if (length >= this->mFlushedOutputData) {
            this->mFlushedOutputData = 0;
        } else {
            this->mFlushedOutputData = this->mFlushedOutputData - length;
        }
        this->mTotalBytesSent += length;
        this->mTotalBytesSentMD5.Update(&pack->mProtocol, length);
        ++this->mNextSequenceNumber;
    } else {
        ++this->mNextSequenceNumber;
        this->mSentShutdown = true;
    }
    return pack;
}

// 0x00488AA0
Moho::SPacket *Moho::CNetUDPConnection::NewGoodbyePacket() {
    return this->NextPacket(7, 0, true);
}

// 0x00488B20
Moho::SPacket *Moho::CNetUDPConnection::NewPacket(Moho::ENetPacketState state, int size, bool inherit) {
    Moho::SPacket *pack = this->mConnector->NewPacket();
    pack->mResendCount = 0;
    pack->mSize = size + 15;
    pack->mState = state;
    pack->mSequenceNumber = inherit ? this->mNextSequenceNumber : 0;
    pack->mExpectedSequenceNumber = inherit ? this->mExpectedSequenceNumber : 0;
    pack->mEarlyMask = inherit ? this->mMask : 0;
    pack->mPayloadLength = size;
    return pack;
}

// 0x00488D80
void Moho::CNetUDPConnection::SendPacket(Moho::SPacket *pack) {
    Moho::SPacketData * dat = pack->Data();
    pack->mSerialNumber = this->mNextSerialNumber;
    pack->mInResponseTo = this->mInResponseTo;
    sockaddr_in to;
    to.sin_family = AF_INET;
    to.sin_port = ::htons(mPort);
    to.sin_addr.S_un.S_addr = ::htonl(this->mAddr);
    ::sendto(this->mConnector->mSocket, &dat->mState, pack->mSize, 0, (SOCKADDR *) &to, sizeof(to));
    LONGLONG time = this->mConnector->GetTime();
    if (Moho::net_LogPackets) {
        func_LogPacket(0, this->mConnector, time, this->mAddr, this->mPort, dat, pack->mSize);
    }
    if (dat->mState == ACK) {
        ++pack->mResendCount;
        pack->mSentTime = time + this->CalcResendDelay(pack);
        Moho::TDatList<Moho::SPacket, void> *i;
        for (i = this->mUnackedPayloads.begin(); i != this->mUnackedPayloads.end(); ++i) {
            if (pack->mSentTime < (*i)->mSentTime) {
                break;
            }
        }
        pack->mList.ListLinkBefore(i);
    } else {
        this->mConnector->DisposePacket(pack);
    }
    this->mConnector->mBuff.Add(0, time, pack->mSize);
    if (Moho::net_DebugLevel >= 2) {
        std::string packStr = pack->ToString();
        std::string thisStr = this->ToString();
        std::string timeStr = func_FileTimeToString(time);
        gpg::Debugf("%s: send %s, %s%s", timeStr.c_str(), thisStr.c_str(), packStr.c_str(), "");
    }
    ++this->mNextSerialNumber;
    this->mInResponseTo = 0;
    if (this->mPendingOutputData.GetLength() > 0) {
        this->mSendBy.mTime = 1000 * Moho::net_SendDelay + this->mConnector->GetTime();
    } else {
        this->mSendBy.mTime = 0;
    }
    this->mTimings[dat->mSerialNumber & 0x7F].mSource = dat->mSerialNumber;
    this->mTimings[dat->mSerialNumber & 0x8000007F].mTime.Reset();
    this->mSendTime = pack->mSize + this->GetBacklog(time);
    this->mLastSend.mTime = time;
    if (dat->mState != KEEPALIVE) {
        this->mLastKeepAlive = time;
    }
    if (dat->mState == GOODBYE) {
        this->mKeepAliveFreq = 2000000;
    }
}


// 0x0048BBE0
Moho::INetConnector *Moho::NET_MakeUDPConnector(u_short port, boost::weak_ptr<Moho::INetNATTraversalProvider> *prov) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        return nullptr;
    }
    u_long argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        }
        ::closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(0);
    if (::bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        if (Moho::net_DebugLevel != 0) {
            gpg::Logf("NET_MakeUDPConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        }
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetUDPConnector{sock, prov};
}
