// implied file

#include "NetTCPConn.h"
#include "winsock.h"

// 0x00485830
void *struct_TCPConnLL::GetPtr() {
    struct_TCPConnLL *next = this->mNext;
    if (next != nullptr) {
        return (void *)(next - 1); // likely a static cast
    } else {
        return nullptr;
    }
}

// inline 0x00484BD5
struct_TCPConnLL::~struct_TCPConnLL() {
    for (struct_TCPConnLL *i = this->mNext; i; i = this->mNext) {
        this->mNext = i[1].mNext;
        i->mNext = nullptr;
        i[1].mNext = nullptr;
    }
}

// 0x004835B0
u_long Moho::CNetTCPConnection::GetAddr() {
    return this->mAddr;
}

// 0x004835C0
u_short Moho::CNetTCPConnection::GetPort() {
    return this->mPort;
}

// 0x004835D0
float Moho::CNetTCPConnection::GetPing() {
    return 100.0;
}

// 0x00484520
float Moho::CNetTCPConnection::GetTime() {
    return gpg::time::CyclesToMilliseconds(this->mTimer1.ElapsedCycles());
}

// 0x00484540
void Moho::CNetTCPConnection::Write(struct_DataSpan *data) {
    this->mPipestream2.Write(data->mStart, data->mEnd - data->mStart);
}

// 0x00484590
void Moho::CNetTCPConnection::Close() {
    this->mPipestream2.VirtClose(gpg::Stream::ModeSend);
}

// 0x004845B0
std::string Moho::CNetTCPConnection::ToString() {
    return gpg::STR_Printf("%s:%d", Moho::NET_GetHostName(this->mAddr), this->mPort);
}

// 0x004835E0
void Moho::CNetTCPConnection::ScheduleDestroy() {
    this->mScheduleDestroy = true;
}

// 0x00483650
Moho::CNetTCPConnection::CNetTCPConnection(Moho::CNetTCPConnector *connector, SOCKET sock, u_long addr, u_short port, int a7) :
    Moho::INetConnection{},
    mConnector{connector},
    mSocket{sock},
    v266{a7},
    mTimer1{},
    mPipestream1{},
    mPipestream2{},
    mSize{0},
    mHasShutdown{false},
    mScheduleDestroy{false}
{
    this->mPrev = connector->mConnections.mPrev;
    this->mNext = &connector->mConnections;
    this->mPrev->mPrev = this;
    this->mPrev->mNext = this;
    if (connector->mHandle) {
        ::WSAEventSelect(sock, connector->mHandle, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}

// 0x004838D0
void Moho::CNetTCPConnection::Push() {
    if (this->mScheduleDestroy) {
        delete(this);
    } else if (! this->mHasShutdown && ! this->mPushFailed && (this->v266 == 3 || this->v266 == 4)) {
        while (true) {
            if (this->mSize < 2048) {
                int avail = this->mPipestream2.GetLength();
                if (avail) {
                    int amt = 2048 - this->mSize;
                    if (amt >= avail ) {
                        amt = avail;
                    }
                    this->mPipestream2.Read(&this->mBuff[this->mSize], amt);
                    this->mSize += amt;
                }
            }
            if (! this->mSize) {
                break;
            }
            int res = ::send(this->mSocket, this->mBuff, this->mSize, 0);
            if (res == SOCKET_ERROR) {
                if (::WSAGetLastError() != 10035) {
                    gpg::Logf("CNetTCPConnection::Push: send() failed: %s", Moho::NET_GetWinsockErrorString());
                    this->mPushFailed = true;
                    if (this->mConnector->mHandle) {
                        ::SetEvent(this->mConnector->mHandle);
                    }
                }
                return;
            }
            if (res < this->mSize) {
                memmov(this->mBuff, &this->mBuff[res], this->mSize - res);
            }
            this->mSize -= res;
        }
        if (this->mPipestream2.Empty()) {
            ::shutdown(this->mSocket, SD_SEND);
            this->mHasShutdown = true;
        }
    }
}

// 0x00483A60
void Moho::CNetTCPConnection::Pull(Moho::TDatListItem<Moho::SPartialConnection, void> *partConn) {
    int v2; // ebx
    struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // eax
    Moho::CNetTCPConnector *connector; // eax
    vtable_stream *p_ll; // eax
    DWORD *p_dtr; // eax
    struct_Datagram *v7; // eax
    std::string *HostName; // eax
    const char *Ptr; // eax
    struct_Datagram *v10; // eax
    char *writeStart; // edx
    size_t v12; // esi
    DWORD *pos; // eax
    unsigned __int16 v14; // dx
    char *v15; // eax
    int v16; // ecx
    std::string *v17; // eax
    const char *v18; // eax
    struct_Datagram *v19; // eax
    void (__thiscall ***v20)(DWORD, struct_Datagram *, Moho::CMessageDispatcher *); // ecx
    size_t Myres; // eax
    DWORD *v22; // eax
    char *v23; // ecx
    int v24; // esi
    bool v25; // zf
    int v26; // esi
    char *v27; // ecx
    char *v28; // eax
    std::string *v29; // eax
    const char *v30; // eax
    void (__thiscall *VirtClose)(gpg::Stream *, int); // edx
    std::string *v32; // eax
    const char *v33; // esi
    const char *WinsockErrorString; // eax
    void (__thiscall *v35)(gpg::Stream *, int); // eax
    struct_Datagram *p_datagram; // ebx
    gpg::PipeStream *p_pipestream1; // edi
    void (__thiscall ***v38)(DWORD); // ecx
    void (__thiscall **v39)(DWORD); // edx
    void (__thiscall *v40)(DWORD); // eax
    std::string *v41; // eax
    const char *v42; // ecx
    Moho::TDatListItem_SPartialConnection *p_next; // eax
    char *originalVec; // eax
    struct_Datagram *v45; // eax
    void (__thiscall ***v46)(DWORD, struct_Datagram *, Moho::CMessageDispatcher *); // ecx
    int *v47; // eax
    struct_Datagram *v48; // eax
    struct_Datagram *v49; // eax
    void (__thiscall ***v50)(DWORD, struct_Datagram *, Moho::CMessageDispatcher *); // ecx
    int v51; // eax
    struct_Datagram *v52; // eax
    size_t v53; // [esp+0h] [ebp-CECh]
    char *start; // [esp+4h] [ebp-CE8h]
    struct_Datagram *p_a3; // [esp+Ch] [ebp-CE0h] BYREF
    Moho::CMessageDispatcher *v56; // [esp+10h] [ebp-CDCh]
    int v57[2]; // [esp+14h] [ebp-CD8h] BYREF
    int v59; // [esp+20h] [ebp-CCCh] BYREF
    gpg::Stream v60; // [esp+24h] [ebp-CC8h] BYREF
    char **v61; // [esp+40h] [ebp-CACh]
    std::string v63; // [esp+5Ch] [ebp-C90h] BYREF
    std::string v64; // [esp+78h] [ebp-C74h] BYREF
    std::string v65; // [esp+94h] [ebp-C58h] BYREF
    std::string a2; // [esp+B0h] [ebp-C3Ch] BYREF
    std::string v67; // [esp+CCh] [ebp-C20h] BYREF
    struct_Datagram v68; // [esp+E8h] [ebp-C04h] BYREF
    int *v69; // [esp+170h] [ebp-B7Ch]
    int *v70; // [esp+174h] [ebp-B78h]
    int v71; // [esp+178h] [ebp-B74h]
    int *v72; // [esp+17Ch] [ebp-B70h]
    int v73; // [esp+180h] [ebp-B6Ch] BYREF
    char *v74; // [esp+1C8h] [ebp-B24h] BYREF
    int v75; // [esp+1CCh] [ebp-B20h]
    struct_Datagram v76; // [esp+1D0h] [ebp-B1Ch] BYREF
    DWORD *v77; // [esp+224h] [ebp-AC8h]
    int a3; // [esp+228h] [ebp-AC4h] BYREF
    DWORD *v79; // [esp+22Ch] [ebp-AC0h]
    struct_Datagram a1; // [esp+270h] [ebp-A7Ch] BYREF
    char buf[2040]; // [esp+4D8h] [ebp-814h] BYREF
    char v84; // [esp+CD0h] [ebp-1Ch]
    char v85; // [esp+CD8h] [ebp-14h]


    v59 = v2;
    if (this->mConnector != nullptr) {
        p_ll = (vtable_stream *)&this->mConnector->mll;
    } else {
        p_ll = 0;
    }
    v60.vtable = p_ll;
    if (p_ll) {
        v60.start = (char *)p_ll->dtr;
        p_ll->dtr = (void (__thiscall *)(gpg::Stream *, int))&v60;
    } else {
        v60.start = 0;
    }
    if (this->mScheduleDestroy) {
        delete(this);
        p_dtr = &v60.vtable->dtr;
        if (! v60.vtable)
            return;
        if ((char *)v60.vtable->dtr != (char *)&v60) {
            do
                p_dtr = (DWORD *)(*p_dtr + 4);
            while ( (gpg::Stream *)*p_dtr != &v60 );
        }
    LABEL_136:
        *p_dtr = v60.mStart;
        return;
    }
    if (this->v266 == 1) {
        fd_set writefds;
        writefds.fd_array[0] = this->mSocket;
        writefds.fd_count = 1;
        fd_set exceptfds;
        exceptfds.fd_array[0] = writefds.fd_array[0];
        exceptfds.fd_count = 1;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if (! ::select(0, 0, &writefds, &exceptfds, &timeout)) {
            p_dtr = &v60.vtable->dtr;
            if (! v60.vtable)
                return;
            if ((char *)v60.vtable->dtr != (char *)&v60) {
                do
                    p_dtr = (DWORD *)(*p_dtr + 4);
                while ( (gpg::Stream *)*p_dtr != &v60 );
            }
            goto LABEL_136;
        }
        if (::_WSAFDIsSet(this->mSocket, &writefds)) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): connection succeeded",
                Moho::NET_GetHostName(this->mAddr).c_str(), this->port);
            LOBYTE(v56) = -55;
            p_a3 = (struct_Datagram *)&a3;
            this->v266 = 3;
            Moho::CMessage v10{0, p_a3, (char)v56};
            this->mPipestream1.Write(v10->mBuff.mStart, v10->mBuff.Size());
            pos = (DWORD *)v76.pos;
            if ((DWORD *)v76.pos != v79) {
                delete[]((void *)v76.pos);
                pos = v79;
                v76.pos = (int)v79;
                a3 = *v79;
            }
            v77 = pos;
        } else if (::_WSAFDIsSet(this->mSocket, &exceptfds)) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): connection failed",
                Moho::NET_GetHostName(this->mAddr).c_str(), this->port);
            LOBYTE(v56) = 200;
            p_a3 = &v68;
            this->v266 = 5;
            v19 = struct_Datagram::struct_Datagram(0, p_a3, (char)v56);
            v20 = (void (__thiscall ***)(DWORD, struct_Datagram *, Moho::CMessageDispatcher *))this->data[*(unsigned __int8 *)v19->buf.start];
            if (v20)
                (**v20)(v20, v19, &this->Moho::CMessageDispatcher);
            v85 = 0;
            Myres = *((DWORD *)&v67._Bx._Ptr + 2);
            if (*((DWORD *)&v67._Bx._Ptr + 2) != v67._Myres) {
                operator delete[](*((void **)&v67._Bx._Ptr + 2));
                Myres = v67._Myres;
                *((DWORD *)&v67._Bx._Ptr + 2) = v67._Myres;
                v67._Mysize = *(DWORD *)v67._Myres;
            }
            *((DWORD *)&v67._Bx._Ptr + 3) = Myres;
            v22 = (DWORD *)v57[0];
            if (v57[0]) {
                if (v57[0] != 4 && this->scheduleDestroy) {
                    delete this;
                    v22 = (DWORD *)v57[0];
                }
                if (v22) {
                    for ( ; (int *)*v22 != v57; v22 = (DWORD *)(*v22 + 4) )
                        ;
                    *v22 = v57[1];
                    }
            }
            return;
        }
        struct_Datagram::struct_Datagram(0, &v76, 0);
        v85 = 5;
        gpg::Stream::Stream(&v60);
        v61 = &v74;
        v60.vtable = &Moho::CMessageStream::`vftable';
        v14 = *(_WORD *)(v74 + 1);
        v15 = v74 + 3;
        if ( v14 >= 3u )
        v16 = v14 - 3;
        else
        v16 = 0;
        v60.readHead = v74 + 3;
        v60.start = v74 + 3;
        v60.end = &v15[v16];
        v60.writeStart = v74 + 3;
        v60.writeHead = v74 + 3;
        v60.dataEnd = &v15[v16];
        v85 = 6;
        v59 = this->mConnector->GetLocalPort();
        if ((unsigned int)(v60.dataEnd - v60.writeStart) < 2) {
            v60->VirtWrite(&v60, (char *)&v59, 2);
        } else {
            *(_WORD *)v60.writeStart = v59;
            v60.writeStart += 2;
        }
        v23 = this->mPipestream2.writeStart;
        v24 = v75 - (DWORD)v74;
        v53 = v75 - (DWORD)v74;
        if (v75 - (int)v74 > (unsigned int)(this->mPipestream2.dataEnd - v23)) {
            this->mPipestream2->VirtWrite(&this->mPipestream2, v74, v53);
        } else {
            memcpy(v23, v74, v53);
            this->mPipestream2.writeStart += v24;
        }
        v85 = 5;
        gpg::Stream::~Stream(&v60);
        v85 = 0;
        if ( v74 != v76.buf.end )
            operator delete[](v74);
    } else if (this->v266 != 3) {
        if (this->v266 == 4 && this->pushFailed) {
            LOBYTE(v56) = -54;
            p_a3 = (struct_Datagram *)&a1.buf.capacity;
            this->v266 = 5;
            v7 = struct_Datagram::struct_Datagram(0, p_a3, (char)v56);
            Moho::CMessageDispatcher::Dispatch((unsigned __int8 **)v7, (int)&this->Moho::CMessageDispatcher);
            struct_Datagram::~struct_Datagram(&a1);
        }
        goto LABEL_17;
    }
    HIBYTE(v60.readHead) = 0;
    if (! this->mPullFailed) {
        while (true) {
            v26 = ::recv(this->mSocket, buf, 2048, 0);
            if (v26 < 0)
                break;
            gpg::time::Timer::Timer(&this->mTimer1);
            if (v26) {
                this->mPipestream1.Write(buf, this->pipestream1.LeftInWriteBuffer());
            } else {
                gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): at end of stream.",
                    Moho::NET_GetHostName(this->addr).c_str(), this->port);
                this->mPipestream1.VirtClose(gpg::Stream::ModeSend);
                this->mPullFailed = true;
            }
            if (this->mPullFailed)
                goto LABEL_84;
        }
        if (::WSAGetLastError() != 10035) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): recv() failed: %s",
                Moho::NET_GetHostName(this->mAddr).c_str(),
                this->mPort,
                Moho::NET_GetWinsockErrorString()
            );
            this->mPipestream1.VirtClose(gpg::Stream::ModeSend);
            this->mPullFailed = true;
        }
    }
    LABEL_84:
    p_datagram = &this->mMessage;
    p_pipestream1 = &this->mPipestream1;
    if (this->mMessage.Read(&this->mPipestream1)) {
        while (true) {
            v38 = (void (__thiscall ***)(DWORD))this->data[*(unsigned __int8 *)p_datagram->buf.start];
            if (v38) {
                v39 = *v38;
                v56 = &this->Moho::CMessageDispatcher;
                v40 = *v39;
                p_a3 = &this->datagram;
                v40(v38);
            } else {
                std::string v41 = Moho::NET_GetHostName(this->addr);
                gpg::Logf("No receiver for message type %d received from %s:%d.",
                    *(unsigned __int8 *)p_datagram->buf.start,
                    v41.c_str(),
                    this->port
                );
            }
            p_next = ebp0;
            if (! ebp0) {
                break;
            }
            if (ebp0 == (Moho::TDatListItem_SPartialConnection *)4) {
                if ((Moho::TDatListItem_SPartialConnection **)MEMORY[4] != &ebp0) {
                    do
                        p_next = (Moho::TDatListItem_SPartialConnection *)&p_next->previous->next;
                    while ( (Moho::TDatListItem_SPartialConnection **)p_next->previous != &ebp0 );
                }
                p_next->previous = (Moho::TDatListItem_SPartialConnection *)v59;
                return;
            }
            if (this->scheduleDestroy) {
                delete this;
                for ( p_dtr = &ebp0->previous;
                    (Moho::TDatListItem_SPartialConnection **)*p_dtr != &ebp0;
                    p_dtr = (DWORD *)(*p_dtr + 4) )
                {
                    ;
                }
                goto LABEL_136;
            }
            if (this->datagram.buf.start != this->datagram.buf.originalVec) {
                operator delete[](p_datagram->buf.start);
                originalVec = this->datagram.buf.originalVec;
                p_datagram->buf.start = originalVec;
                this->datagram.buf.capacity = *(char **)originalVec;
            }
            this->datagram.buf.end = this->datagram.buf.start;
            this->datagram.pos = 0;
            if (! this->datagram.Read(p_pipestream1))
                goto LABEL_99;
        }
    } else {
    LABEL_99:
        if (HIBYTE(v60.vtable) || this->pushFailed) {
            this->v266 = 5;
            v49 = struct_Datagram::struct_Datagram(0, (struct_Datagram *)&v68.buf.inlineVec[56], 202);
            v85 = 10;
            v50 = (void (__thiscall ***)(DWORD, struct_Datagram *, Moho::CMessageDispatcher *))this->data[*(unsigned __int8 *)v49->buf.start];
            if ( v50 )
                (**v50)(v50, v49, &this->Moho::CMessageDispatcher);
            v84 = 0;
            v51 = *(DWORD *)&v68.buf.inlineVec[40];
            if ( *(DWORD *)&v68.buf.inlineVec[40] != *(DWORD *)&v68.buf.inlineVec[52]) {
                operator delete[](*(void **)&v68.buf.inlineVec[40]);
                v51 = *(DWORD *)&v68.buf.inlineVec[52];
                *(DWORD *)&v68.buf.inlineVec[40] = *(DWORD *)&v68.buf.inlineVec[52];
                *(DWORD *)&v68.buf.inlineVec[48] = **(DWORD **)&v68.buf.inlineVec[52];
            }
            *(DWORD *)&v68.buf.inlineVec[44] = v51;
            v52 = p_a3;
            if (p_a3) {
                if ( p_a3 != (struct_Datagram *)4 && this->scheduleDestroy) {
                    delete this;
                    v52 = p_a3;
                }
                if ( 52) {
                    for ( ; (struct_Datagram **)v52->buf.start != &p_a3; v52 = (struct_Datagram *)(v52->buf.start + 4))
                        ;
                    v52->buf.start = (char *)v56;
                }
            }
        } else {
            if (this->pipestream1.readHead != this->pipestream1.end || !p_pipestream1->VirtAtEnd()) {
        LABEL_17:
                p_dtr = &ebp0->previous;
                if (! ebp0) {
                    return;
                }
                if ((Moho::TDatListItem_SPartialConnection **)ebp0->previous != &ebp0) {
                    do
                        p_dtr = (DWORD *)(*p_dtr + 4);
                    while ( (Moho::TDatListItem_SPartialConnection **)*p_dtr != &ebp0 );
                }
                goto LABEL_136;
            }
            this->v266 = 4;
            v45 = struct_Datagram::struct_Datagram(0, (struct_Datagram *)&v73, 203);
            v85 = 11;
            v46 = (void (__thiscall ***)(DWORD, struct_Datagram *, Moho::CMessageDispatcher *))this->data[*(unsigned __int8 *)v45->buf.start];
            if ( v46 )
                (**v46)(v46, v45, &this->Moho::CMessageDispatcher);
            v84 = 0;
            v47 = v69;
            if (v69 != v72) {
                operator delete[](v69);
                v47 = v72;
                v69 = v72;
                v71 = *v72;
            }
            v70 = v47;
            v48 = p_a3;
            if (p_a3) {
                if (p_a3 != (struct_Datagram *)4 && this->scheduleDestroy) {
                    delete this;
                    v48 = p_a3;
                }
                if (v48) {
                    for ( ; (struct_Datagram **)v48->buf.start != &p_a3; v48 = (struct_Datagram *)(v48->buf.start + 4) )
                        ;
                    v48->buf.start = (char *)v56;
                }
            }
        }
    }
}


// 0x00484AB0
Moho::CNetTCPConnector::CNetTCPConnector(SOCKET sock)
    : Moho::INetConnector{}, mSocket{sock}
{}

// 0x00484AE0
Moho::CNetTCPConnector::~CNetTCPConnector() {
    while (! this->mConnections.ListEmpty()) {
        Moho::CNetTCPConnection *conn = *this->mConnections.begin();
        if (conn != nullptr) {
            delete(conn);
        }
    }
    if (this->mSocket != INVALID_SOCKET) {
        closesocket(this->mSocket);
    }
}

// 0x00483600
void Moho::CNetTCPConnector::Destroy() {
    delete(this);
}

// 0x00483610
Moho::ENetProtocol Moho::CNetTCPConnector::GetProtocol() {
    return Moho::NETPROTO_TCP;
}

// 0x00484C20
int Moho::CNetTCPConnector::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getsockname(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}

// 0x00484C50
Moho::INetConnection *Moho::CNetTCPConnector::Connect(u_long addr, u_short port) {
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): socket() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    int argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetHostName(addr), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(addr);
    if (::connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR && WSAGetLastError() != 10035) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): connect() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    gpg::Logf("CNetTCPConnector::Connect(%s:%d)...", Moho::NET_GetHostName(addr).c_str(), port);
    return new Moho::CNetTCPConnection(this, sock, addr, port, 1);
}

// 0x00484EA0
bool Moho::CNetTCPConnector::FindNextAddr(__out u_long &addr, __out u_short &port) {
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection *conn = *cur;
        if (conn->v266 != 0) {
            addr = conn->GetAddr();
            port = conn->GetPort();
            return true;
        }
    }
    return false;
}

// 0x00484F00
Moho::INetConnection *Moho::CNetTCPConnector::Accept(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Accept(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection * conn = *cur;
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 != 0) {
            conn->v266 = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection(this, INVALID_SOCKET, addr, port, 2);
}

// 0x00485050
void Moho::CNetTCPConnector::Reject(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Reject(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection * conn = *cur;
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 != 0) {
            conn->ScheduleDestroy();
            return;
        }
    }
    gpg::Logf("CNetTCPConnector::Reject(%s:%d): No such connection pending.", Moho::NET_GetHostName(addr).c_str(), port);
}

// 0x004838D0
void Moho::CNetTCPConnector::Pull() {
    sockaddr_in addr;
    int addrlen = sizeof(addr);
    SOCKET sock = ::accept(this->mSocket, (SOCKADDR *) &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        if (::WSAGetLastError() != 10035) {
            gpg::Logf("CNetTCPConnector::Pull: accept() failed: %s", Moho::NET_GetWinsockErrorString());
        }
    } else {
        u_long host = ::ntohl(addr.sin_addr.S_un.S_addr);
        u_short port = ::ntohs(addr.sin_port);
        gpg::Logf("CNetTCPConnector::Pull(): accepted connection from %s:%d", Moho::NET_GetHostName(host).c_str(), port);
        auto partConn = new Moho::SPartialConnection{sock, host, this, port};
        partConn->mPrev->mNext = partConn->mNext;
        partConn->mNext->mPrev = partConn->mPrev;
        partConn->mPrev = partConn;
        partConn->mNext = partConn;
        partConn->mPrev = this->mPartialConns.mPrev;
        partConn->mNext = &this->mPartialConns;
        this->partialConns.mPrev = partConn;
        partConn->mPrev->mNext = partConn;
    }

    struct_TCPConnLL *curLL = &this->mll;
    struct_TCPConnLL workingLL;
    workingLL.mNext = curLL;
    struct_TCPConnLL *startingLL = curLL->mNext;
    curLL->mNext = &workingLL;

    if (this->partialConns.next != &this->partialConns) {
        for (auto next = this->mPartialConns.begin(); next != this->mPartialConns.end(); ++next) {
            next->Pull();
        }
        curLL = workingLL.mNext;
    }
    for (auto next = this->mConnections.begin(); next != this->mConnections.end(); ++next) {
        Moho::CNetTCPConnection *conn = *next;
        conn->Pull(&this->mPartialConns);
        if (! workingLL.GetPtr()) {
            curLL = workingLL.mNext;
            break;
        }
    }

    if (curLL) {
        for ( ; curLL->mNext != &workingLL; curLL = curLL->mNext + 1)
            {}
        curLL->mNext = startingLL;
    }
}

// 0x00485610
void Moho::CNetTCPConnector::Push() {
    for (auto cur = this->mConnections.begin(); cur != this->connections.end(); ++cur) {
        cur->Push();
    }
}

// 0x00485640
void Moho::CNetTCPConnector::SelectEvent(HANDLE ev) {
    ::WSAEventSelect(this->mSocket, ev, FD_ACCEPT);
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        ::WSAEventSelect(cur->mSocket, ev, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}

// 0x00483620
struct_a3 Moho::CNetTCPConnector::Func3(LONGLONG since) {
    return struct_a3{};
}

// 0x004853D0
bool Moho::CNetTCPConnector::ReadFromStream(SOCKET s, u_long addr, u_short port, gpg::PipeStream *strm) {
    Moho::CNetTCPConnection *conn = this->GetConnection(s, addr, port);
    conn->mPipestream1.Write(Moho::CMessage{0, 201}.buf); // ?
    strm->VirtClose(gpg::Stream::ModeSend);
    while (strm->mReadHead != strm->mEnd && ! strm->VirtAtEnd()) {
        char buf[2048];
        strm->Read(buf, sizeof(buf));
        conn->mPipestream1.Write(buf, sizeof(buf));
    }
    return true;
}

// inline 0x004853F8
Moho::CNetTCPConnection *Moho::CNetTCPConnector::GetConnection(SOCKET s, u_long addr, u_short port) {
    for (auto cur = this->mConnections.begin(); cur != this->mConnections.end(); ++cur) {
        Moho::CNetTCPConnection * conn = *cur;
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 == 2) {
            conn->mSocket = s;
            conn->v266 = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection{this, s, addr, port, 0};
}

// 0x004846E0
Moho::SPartialConnection::~SPartialConnection() {
    if (this->mSocket != INVALID_SOCKET) {
        ::closesocket(this->mSocket);
    }
}

// 0x00484770
void Moho::SPartialConnection::Pull() {
    char buf[2048];

    int res = ::recv(this->mSocket, buf, sizeof(buf), 0);
    if (res < 0) {
        delete(this);
        return;
    }
    while (res != 0) {
        this->mStream.Write(buf, res);
        if (this->mBuff.Read(&this->mStream)) {
            Moho::CMessageStream msg{&this->mBuff};
            gpg::BinaryReader v10{&msg};
            v10.Read(&v10.mPort, 2);
            this->mConnector->ReadFromStream(this->mSocket, this->mAddr, v10.mPort, &this->mStream);
            this->mSocket = INVALID_SOCKET;
            delete(this);
            return;
        }
        res = ::recv(this->mSocket, buf, sizeof(buf), 0);
        if (res < 0) {
            if (::WSAGetLastError() == 10035) {
                return;
            }
            gpg::Logf("SPartialConnection<%s:%d>::Pull(): recv() failed: %s",
                Moho::NET_GetHostName(this->mAddr).c_str(),
                this->mPort,
                Moho::NET_GetWinsockErrorString()
            );
            delete(this);
            return;
        }
    }
    gpg::Logf("SPartialConnection<%s:%d>::Pull(): at end of stream.",
            Moho::NET_GetHostName(this->mAddr).c_str(), this->mPort);
    delete(this);
}


// 0x004827C0
Moho::CNetTCPBuf::~CNetTCPBuf() {
    this->VirtClose(this->mMode);
}

// 0x00482A90
unsigned int Moho::CNetTCPBuf::VirtRead(char *buf, unsigned int len) {
    return this->Read(buf, len, true);
}

// 0x00482AB0
unsigned int Moho::CNetTCPBuf::VirtReadNonBlocking(char *buf, unsigned int len) {
    return this->Read(buf, len, false);
}

// 0x00482AD0
bool Moho::CNetTCPBuf::VirtAtEnd() {
    if (this->mSocket = INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    return this->mFailed;
}

// 0x00482B50
void Moho::CNetTCPBuf::VirtWrite(const char *buf, unsigned int len) {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if ((this->mMode & gpg::Stream::ModeSend) == 0) {
        throw std::runtime_error{std::string{"output shutdown"}};
    }
    if (len != 0) {
        this->Write(buf, len);
    }
}

// 0x00482CE0
void Moho::CNetTCPBuf::VirtFlush() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    int len = this->mWriteStart - this->mWriteHead;
    if (len != 0)  {
        if (::send(socket, &this->mBuffer[2048], len, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Flush(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        this->mWriteStart = &this->mBuffer[2048];
    }
}

// 0x00482DA0
void Moho::CNetTCPBuf::VirtClose(gpg::Stream::Mode mode) {
    if (this->socket != INVALID_SOCKET) {
        if ((this->mMode & mode & gpg::Stream::ModeSend) != 0) {
            this->VirtFlush();
            ::shutdown(this->mSocket, SD_SEND);
            this->mMode &=~ gpg::Stream::ModeSend;
            this->mDataEnd = 0;
            this->mWriteStart = 0;
            this->mWriteHead = 0;
        }
        if ((this->mMode & mode & gpg::Stream::ModeReceive) != 0) {
            ::shutdown(this->mSocket, SD_RECEIVE);
            this->mMode &=~ gpg::Stream::ModeReceive;
            this->mEnd = 0;
            this->mReadHead = 0;
            this->mStart = 0;
        }
        if (this->mMode != gpg::Stream::ModeNone) {
            ::closesocket(this->mSocket);
            this->mSocket = INVALID_SOCKET;
        }
    }
}

// 0x00482880
u_short Moho::CNetTCPBuf::GetPort() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getsockname(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}

// 0x00482930
u_long Moho::CNetTCPBuf::GetPeerAddr() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getpeername(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohl(name.sin_addr.S_un.S_addr);
}

// 0x004829E0
u_short Moho::CNetTCPBuf::GetPeerPort() {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getpeername(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}


// 0x00482770
Moho::CNetTCPBuf::CNetTCPBuf(SOCKET sock) :
    Moho::INetTCPSocket{},
    mSocket{sock}    
{}

// 0x00482E20
int Moho::CNetTCPBuf::Read(char *buf, unsigned int amt, bool isBlocking) {
    if (this->mSocket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if (this->mFailed || amt == 0) {
        return 0;
    }
    int avail = this->mEnd - this->mReadHead;
    int totalRead = 0;
    while (amt > avail) {
        if (avail) {
            memcpy(buf, this->mReadHead, avail);
            buf += avail;
            amt -= avail;
            totalRead += avail;
        }
        this->mEeadHead = this->mBuffer;
        this->mEnd = this->mBuffer;
        while (buf && amt >= 2048) {
            if (! isBlocking) {
                fd_set readfds;
                readfds.fd_array[0] = this->socket;
                readfds.fd_count = 1;
                timeval timeout;
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
                int res = ::select(1, &readfds, 0, 0, &timeout);
                if (res != SOCKET_ERROR && res <= 0) {
                    return totalRead;
                }
            }
            int res = ::recv(this->mSocket, buf, amt, 0);
            if (res == SOCKET_ERROR) {
                gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
                this->mFailed = true;
                return totalRead;
            }
            if (res == 0) {
                this->mFailed = true;
                return totalRead;
            }
            buf += res;
            amt -= res;
            if (amt == 0) {
                return totalRead;
            }
        }
        if (! isBlocking && ! this->Select()) {
            return totalRead;
        }
        int res = ::recv(this->mSocket, this->mBuffer, 2048, 0);
        if (res == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
            this->mFailed = true;
            return totalRead;
        }
        if (res == 0) {
            this->mFailed = true;
            return totalRead;
        }
        this->mEnd = &this->mBuffer[res];
        avail = this->mEnd - this->mReadHead;
    }
    memcpy(buf, this->mReadHead, amt);
    this->mReadHead += amt;
    return amt + totalRead;
}

// inline 0x00482C0A
void Moho::CNetTCPBuf::Write(const char *buf, unsigned int size) {
    if (size > this->LeftInWriteBuffer()) {
        
        int left = this->LeftInWriteBuffer();
        if (left != 0) {
            memcpy(writeStart, buf, left);
            this->mWriteStart += left;
            buf += left;
            size -= left;
        }
        this->VirtFlush();
        if (size < 2048) {
            memcpy(&this->mBuffer[2048], buf, size);
            this->mWriteStart = &this->mBuffer[size + 2048];
        } else if (send(this->mSocket, buf, size, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Write(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }

    } else {
        memcpy(this->mWriteStart, buf, size);
        this->mWriteStart += size;
    }
}

// 0x00483040     
BOOL Moho::CNetTCPBuf::Select() {
    fd_set readfds;
    readfds.fd_array[0] = this->mSocket;
    readfds.fd_count = 1;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int res = ::select(1, &readfds, 0, 0, &timeout);
    return res == SOCKET_ERROR || res > 0;
}



Moho::CNetTCPServerImpl::CNetTCPServerImpl(SOCKET sock) :
    Moho::INetTCPServer{},
    mSocket{sock}
{}

// 0x00483220
Moho::CNetTCPServerImpl::~CNetTCPServerImpl() {
    if (this->mSocket != INVALID_SOCKET) {
        ::closesocket(this->mSocket);
        this->mSocket = INVALID_SOCKET;
    }
}

// 0x004832A0
unsigned short Moho::CNetTCPServerImpl::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    ::getsockname(this->mSocket, (SOCKADDR *) &name, &namelen);
    return ::ntohs(name.sin_port);
}

// 0x004832D0
Moho::INetTCPSocket *Moho::CNetTCPServerImpl::Accept() {
    SOCKADDR addr;
    int addrlen = sizeof(addr);
    SOCKET sock = ::accept(this->mSocket, &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPServerImpl::Accept(): accept() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}

// 0x00483370
void Moho::CNetTCPServerImpl::CloseSocket() {
    if (this->mSocket != INVALID_SOCKET) {
        ::closesocket(this->mSocket);
        this->mSocket = INVALID_SOCKET;
    }
}


// 0x004830A0
Moho::INetTCPSocket *Moho::NET_TCPConnect(u_long addr, u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_TCPConnect(%s:%d): socket() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(addr);
    if (::connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_TCPConnect(%s:%d): connect() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}

// 0x00483390
Moho::INetTCPServer *Moho::NET_CreateTCPServer(u_long addr, u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_CreateTCPServer(%d): socket() failed: %s", port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(addr);
    if (::bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): bind() failed: %s", port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    if (::listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): listen() failed: %s", port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPServerImpl{sock};
}

// 0x004849A0
Moho::INetConnector *Moho::NET_MakeTCPConnector(u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_MakeConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    unsigned long argp = 1;
    if (::ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = ::htons(port);
    name.sin_addr.S_un.S_addr = ::htonl(0);
    if (::bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    if (::listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: listen() failed: %s", Moho::NET_GetWinsockErrorString());
        ::closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPConnector{sock};
}
