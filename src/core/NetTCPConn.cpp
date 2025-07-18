#include "NetTCPConn.h"

void *struct_TCPConnLL::GetPtr() {
    struct_TCPConnLL *next = this->next;
    if (next != nullptr) {
        return (void *)(next - 1); // likely a static cast
    } else {
        return nullptr;
    }
}

int Moho::CNetTCPConnection::GetAddr() {
    return this->addr;
}
int Moho::CNetTCPConnection::GetPort() {
    return this->port;
}
float Moho::CNetTCPConnection::GetPing() {
    return 100.0;
}
float Moho::CNetTCPConnection::GetTime() {
    return gpg::time::CyclesToMilliseconds(this->timer1.ElapsedCycles());
}
void Moho::CNetTCPConnection::Write(struct_DataSpan *data) {
    this->pipestream2.Write(data->start, data->end - data->start);
}
void Moho::CNetTCPConnection::Close() {
    this->pipestream2.VirtClose(gpg::Stream::ModeSend);
}
std::string Moho::CNetTCPConnection::ToString() {
    return gpg::STR_Printf("%s:%d", Moho::NET_GetHostName(this->addr), this->port);
}
void Moho::CNetTCPConnection::ScheduleDestroy() {
    this->scheduleDestroy = true;
}
Moho::CNetTCPConnection::CNetTCPConnection(Moho::CNetTCPConnector *connector, SOCKET sock, u_long addr, u_short port, int a7) :
    Moho::INetConnection{},
    connector{connector},
    socket{sock},
    v266{a7},
    timer1{},
    pipestream1{},
    pipestream2{},
    size{0},
    hasShutdown{false},
    scheduleDestroy{false}
{
    this->prev = connector->connections.prev;
    this->next = &connector->connections;
    this->prev->prev = this;
    this->prev->next = this;
    if (connector->handle) {
        WSAEventSelect(sock, connector->handle, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}

void Moho::CNetTCPConnection::Push() {
    if (this->scheduleDestroy) {
        delete this;
    } else if (! this->hasShutdown && ! this->pushFailed && (this->v266 == 3 || this->v266 == 4)) {
        while (true) {
            if (this->size < 2048) {
                int avail = this->pipestream2.GetLength();
                if (avail) {
                    int amt = 2048 - this->size;
                    if (amt >= avail ) {
                        amt = avail;
                    }
                    this->pipestream2.Read(&this->buf[this->size], amt);
                    this->size += amt;
                }
            }
            if (! this->size) {
                break;
            }
            int res = send(this->socket, this->buf, this->size, 0);
            if (res == SOCKET_ERROR) {
                if (WSAGetLastError() != 10035) {
                    gpg::Logf("CNetTCPConnection::Push: send() failed: %s", Moho::NET_GetWinsockErrorString());
                    this->pushFailed = true;
                    if (this->connector->handle) {
                        SetEvent(this->connector->handle);
                    }
                }
                return;
            }
            if (res < this->size) {
                memmov(this->buf, &this->buf[res], this->size - res);
            }
            this->size -= res;
        }
        if (this->pipestream2.Empty()) {
            shutdown(this->socket, SD_SEND);
            this->hasShutdown = true;
        }
    }
}

void Moho::CNetTCPConnection::Pull(Moho::TDatListItem<Moho::SPartialConnection> *partConn) {
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
    if (this->connector)
        p_ll = (vtable_stream *)&this->connector->ll;
    else
        p_ll = 0;
    v60.vtable = p_ll;
    if (p_ll) {
        v60.start = (char *)p_ll->dtr;
        p_ll->dtr = (void (__thiscall *)(gpg::Stream *, int))&v60;
    } else {
        v60.start = 0;
    }
    if (this->scheduleDestroy) {
        delete this;
        p_dtr = &v60.vtable->dtr;
        if (! v60.vtable)
            return;
        if ((char *)v60.vtable->dtr != (char *)&v60) {
            do
                p_dtr = (DWORD *)(*p_dtr + 4);
            while ( (gpg::Stream *)*p_dtr != &v60 );
        }
    LABEL_136:
        *p_dtr = v60.start;
        return;
    }
    if (this->v266 == 1) {
        fd_set writefds;
        writefds.fd_array[0] = this->socket;
        writefds.fd_count = 1;
        fd_set exceptfds;
        exceptfds.fd_array[0] = writefds.fd_array[0];
        exceptfds.fd_count = 1;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if (! select(0, 0, &writefds, &exceptfds, &timeout)) {
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
        if (_WSAFDIsSet(this->socket, &writefds)) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): connection succeeded",
                Moho::NET_GetHostName(this->addr).c_str(), this->port);
            LOBYTE(v56) = -55;
            p_a3 = (struct_Datagram *)&a3;
            this->v266 = 3;
            struct_Datagram v10{0, p_a3, (char)v56};
            this->pipestream1.Write(&this->pipestream1, v10->buf.start, v10->buf.Size());
            pos = (DWORD *)v76.pos;
            if ((DWORD *)v76.pos != v79) {
                operator delete[]((void *)v76.pos);
                pos = v79;
                v76.pos = (int)v79;
                a3 = *v79;
            }
            v77 = pos;
        } else if (_WSAFDIsSet(this->socket, &exceptfds)) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): connection failed",
                Moho::NET_GetHostName(this->addr).c_str(), this->port);
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
        v59 = this->connector->GetLocalPort();
        if ((unsigned int)(v60.dataEnd - v60.writeStart) < 2) {
            v60->VirtWrite(&v60, (char *)&v59, 2);
        } else {
            *(_WORD *)v60.writeStart = v59;
            v60.writeStart += 2;
        }
        v23 = this->pipestream2.writeStart;
        v24 = v75 - (DWORD)v74;
        v53 = v75 - (DWORD)v74;
        if (v75 - (int)v74 > (unsigned int)(this->pipestream2.dataEnd - v23)) {
            this->pipestream2->VirtWrite(&this->pipestream2, v74, v53);
        } else {
            memcpy(v23, v74, v53);
            this->pipestream2.writeStart += v24;
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
    if (! this->pullFailed) {
        while (true) {
            v26 = recv(this->socket, buf, 2048, 0);
            if (v26 < 0)
                break;
            gpg::time::Timer::Timer(&this->timer1);
            if (v26) {
                this->pipestream1.Write(buf, this->pipestream1.LeftInWriteBuffer());
            } else {
                gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): at end of stream.",
                    Moho::NET_GetHostName(this->addr).c_str(), this->port);
                this->pipestream1.VirtClose(gpg::Stream::ModeSend);
                this->pullFailed = true;
            }
            if (this->pullFailed)
                goto LABEL_84;
        }
        if (WSAGetLastError() != 10035) {
            gpg::Logf("CNetTCPConnection<%s:%d>::Pull(): recv() failed: %s",
                Moho::NET_GetHostName(this->addr).c_str(),
                this->port,
                Moho::NET_GetWinsockErrorString()
            );
            this->pipestream1.VirtClose(gpg::Stream::ModeSend);
            this->pullFailed = true;
        }
    }
    LABEL_84:
    p_datagram = &this->datagram;
    p_pipestream1 = &this->pipestream1;
    if (this->datagram.Read(&this->pipestream1)) {
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


Moho::CNetTCPConnector::CNetTCPConnector(SOCKET sock)
    : Moho::INetConnector{}, socket{sock}
{}
Moho::CNetTCPConnector::~CNetTCPConnector() {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection *conn = cur->Get();
        if (conn != nullptr) {
            delete conn;
        }
    }
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
    }
    // TDatItemList dtrs
    for (struct_TCPConnLL *i = this->ll.next; i; i = this->ll.next) {
        this->ll = i[1];
        i->next = 0;
        i[1].next = 0;
    }
}

void Moho::CNetTCPConnector::Destroy() {
    delete this;
}
Moho::ENetProtocol Moho::CNetTCPConnector::GetProtocol() {
    return Moho::NETPROTO_TCP;
}
int Moho::CNetTCPConnector::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}
Moho::INetConnection *Moho::CNetTCPConnector::Connect(u_long addr, u_short port) {
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): socket() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    int argp = 1;
    if (ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetHostName(addr), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(addr);
    if (connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR && WSAGetLastError() != 10035) {
        gpg::Logf("CNetTCPConnector::Connect(%s:%d): connect() failed: %s", Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    gpg::Logf("CNetTCPConnector::Connect(%s:%d)...", Moho::NET_GetHostName(addr).c_str(), port);
    return new Moho::CNetTCPConnection(this, sock, addr, port, 1);
}
bool Moho::CNetTCPConnector::Func2(OUT u_long &addr, OUT u_short &port) {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection *conn = cur->Get();
        if (conn->v266 != 0) {
            addr = conn->GetAddr();
            port = conn->GetPort();
            return true;
        }
    }
    return false;
}
Moho::INetConnection *Moho::CNetTCPConnector::Accept(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Accept(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection * conn = cur->Get();
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 != 0) {
            conn->v266 = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection(this, INVALID_SOCKET, addr, port, 2);
}
void Moho::CNetTCPConnector::Reject(u_long addr, u_short port) {
    gpg::Logf("CNetTCPConnector::Reject(%s:%d)", Moho::NET_GetHostName(addr).c_str(), port);
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection * conn = cur->Get();
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 != 0) {
            conn->ScheduleDestroy();
            return;
        }
    }
    gpg::Logf("CNetTCPConnector::Reject(%s:%d): No such connection pending.", Moho::NET_GetHostName(addr).c_str(), port);
}
void Moho::CNetTCPConnector::Pull() {
    sockaddr_in addr;
    int addrlen = sizeof(addr);
    SOCKET sock = accept(this->socket, (SOCKADDR *) &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        if (WSAGetLastError() != 10035) {
            gpg::Logf("CNetTCPConnector::Pull: accept() failed: %s", Moho::NET_GetWinsockErrorString());
        }
    } else {
        u_long host = ntohl(addr.sin_addr.S_un.S_addr);
        u_short port = ntohs(addr.sin_port);
        gpg::Logf("CNetTCPConnector::Pull(): accepted connection from %s:%d", Moho::NET_GetHostName(host).c_str(), port);
        Moho::SPartialConnection * partConn = new Moho::SPartialConnection(sock, host, this, port);
        partConn->prev->next = partConn->next;
        partConn->next->prev = partConn->prev;
        partConn->prev = partConn;
        partConn->next = partConn;
        partConn->prev = this->partialConns.prev;
        partConn->next = &this->partialConns;
        this->partialConns.prev = partConn;
        partConn->prev->next = partConn;
    }

    struct_TCPConnLL *curLL = &this->ll;
    struct_TCPConnLL workingLL;
    workingLL.next = curLL;
    struct_TCPConnLL *startingLL = curLL->next;
    curLL->next = &workingLL;

    if (this->partialConns.next != &this->partialConns) {
        for (auto next = this->partialConns.next; next != &this->partialConns; next = next->next) {
            next->Get()->Pull();
        }
        curLL = workingLL.next;
    }
    for (auto next = this->connections.next; next != &this->connections; next = next->next) {
        Moho::CNetTCPConnection *conn = next->Get();
        conn->Pull(&this->partialConns);
        if (! workingLL.GetPtr()) {
            curLL = workingLL.next;
            break;
        }
    }

    if (curLL) {
        for ( ; curLL->next != &workingLL; curLL = curLL->next + 1)
            ;
        curLL->next = startingLL;
    }
}
void Moho::CNetTCPConnector::Push() {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        cur->Get()->Push();
    }
}
void Moho::CNetTCPConnector::SelectEvent(HANDLE ev) {
    WSAEventSelect(this->socket, ev, FD_ACCEPT);
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        WSAEventSelect(cur->Get()->socket, ev, FD_READ|FD_CONNECT|FD_CLOSE);
    }
}
void *Moho::CNetTCPConnector::Func3() {
    return nullptr; // unknown type;
}


Moho::CNetTCPConnection *Moho::CNetTCPConnector::GetConnection(SOCKET s, u_long addr, u_short port) {
    for (auto cur = this->connections.next; cur != &this->connections; cur = cur->next) {
        Moho::CNetTCPConnection * conn = cur->Get();
        if (conn->GetAddr() == addr && conn->GetPort() == port && conn->v266 == 2) {
            conn->socket = s;
            conn->v266 = 3;
            return conn;
        }
    }
    return new Moho::CNetTCPConnection{this, s, addr, port, 0};
}

bool Moho::CNetTCPConnector::ReadFromStream(SOCKET s, u_long addr, u_short port, gpg::PipeStream *strm) {
    Moho::CNetTCPConnection *conn = this->GetConnection(s, addr, port);
    conn->pipestream1.Write(struct_Datagram{0, 201}.buf); // ?
    strm->VirtClose(gpg::Stream::ModeSend);
    while (strm->readHead != strm->end && ! strm->VirtAtEnd()) {
        char buf[2048];
        strm->Read(buf, sizeof(buf));
        conn->pipestream1.Write(buf, sizeof(buf));
    }
    return true;
}

Moho::SPartialConnection::~SPartialConnection() {
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
    }
}

void Moho::SPartialConnection::Pull() {
    char buf[2048];

    int res = recv(this->socket, buf, sizeof(buf), 0);
    if (res < 0) {
        delete this;
        return;
    }
    while (res != 0) {
        this->stream.Write(buf, res);
        if (this->buf.Read(&this->stream)) {
            Moho::CMessageStream msg{&this->buf};
            gpg::BinaryReader v10{&msg};
            v10.Read(&v10.port, 2);
            this->connector->ReadFromStream(this->socket, this->addr, v10.port, &this->stream);
            this->socket = INVALID_SOCKET;
            delete this;
            return;
        }
        res = recv(this->socket, buf, sizeof(buf), 0);
        if (res < 0) {
            if (WSAGetLastError() == 10035) {
                return;
            }
            gpg::Logf("SPartialConnection<%s:%d>::Pull(): recv() failed: %s",
                Moho::NET_GetHostName(this->addr).c_str(),
                this->port,
                Moho::NET_GetWinsockErrorString()
            );
            delete this;
            return;
        }
    }
    gpg::Logf("SPartialConnection<%s:%d>::Pull(): at end of stream.",
        Moho::NET_GetHostName(this->addr).c_str(), this->port);
    delete this;
}


Moho::CNetTCPServerImpl::CNetTCPServerImpl(SOCKET sock) 
    : Moho::INetTCPServer{}
{
    this->socket = sock;
}
Moho::CNetTCPServerImpl::~CNetTCPServerImpl() {
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
        this->socket = INVALID_SOCKET;
    }
}
Moho::INetTCPSocket *Moho::CNetTCPServerImpl::Accept() {
    SOCKADDR addr;
    int addrlen = sizeof(addr);
    SOCKET sock = accept(this->socket, &addr, &addrlen);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("CNetTCPServerImpl::Accept(): accept() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}
unsigned short Moho::CNetTCPServerImpl::GetLocalPort() {
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}
void Moho::CNetTCPServerImpl::CloseSocket() {
    if (this->socket != INVALID_SOCKET) {
        closesocket(this->socket);
        this->socket = INVALID_SOCKET;
    }
}

Moho::CNetTCPBuf::CNetTCPBuf(SOCKET sock)
    : Moho::INetTCPSocket{}
{
    this->socket = sock;    
}
int Moho::CNetTCPBuf::Read(char *buf, unsigned int amt, bool isBlocking) {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if (this->failed || ! amt) {
        return 0;
    }
    int avail = this->end - this->readHead;
    int totalRead = 0;
    while (amt > avail) {
        if (avail) {
            memcpy(buf, this->readHead, avail);
            buf += avail;
            amt -= avail;
            totalRead += avail;
        }
        this->readHead = this->buffer;
        this->end = this->buffer;
        while (buf && amt >= 2048) {
            if (! isBlocking) {
                fd_set readfds;
                readfds.fd_array[0] = this->socket;
                readfds.fd_count = 1;
                timeval timeout;
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
                int res = select(1, &readfds, 0, 0, &timeout);
                if (res != SOCKET_ERROR && res <= 0) {
                    return totalRead;
                }
            }
            int res = recv(this->socket, buf, amt, 0);
            if (res == SOCKET_ERROR) {
                gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
                this->failed = true;
                return totalRead;
            }
            if (res == 0) {
                this->failed = true;
                return totalRead;
            }
            buf += res;
            amt -= res;
            if (! amt) {
                return totalRead;
            }
        }
        if (! isBlocking && ! this->Select()) {
            return totalRead;
        }
        int res = recv(this->socket, this->buffer, 2048, 0);
        if (res == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Read(): recv() failed: %s", Moho::NET_GetWinsockErrorString());
            this->failed = true;
            return totalRead;
        }
        if (res == 0) {
            this->failed = true;
            return totalRead;
        }
        this->end = &this->buffer[res];
        avail = this->end - this->readHead;
    }
    memcpy(buf, this->readHead, amt);
    this->readHead += amt;
    return amt + totalRead;
}
void Moho::CNetTCPBuf::Write(const char *buf, unsigned int size) {
    if (size > this->LeftInWriteBuffer()) {
        
        int left = this->LeftInWriteBuffer();
        if (left) {
            memcpy(writeStart, buf, left);
            this->writeStart += left;
            buf += left;
            size -= left;
        }
        this->VirtFlush();
        if (size < 2048) {
            memcpy(&this->buffer[2048], buf, size);
            this->writeStart = &this->buffer[size + 2048];
        } else if (send(this->socket, buf, size, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Write(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }

    } else {
        memcpy(this->writeStart, buf, size);
        this->writeStart += size;
    }
}

BOOL Moho::CNetTCPBuf::Select() {
    fd_set readfds;
    readfds.fd_array[0] = this->socket;
    readfds.fd_count = 1;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    int res = select(1, &readfds, 0, 0, &timeout);
    return res == SOCKET_ERROR || res > 0;
}

Moho::CNetTCPBuf::~CNetTCPBuf() {
    this->VirtClose(this->mode);
}

unsigned int Moho::CNetTCPBuf::VirtRead(char *buf, unsigned int len) {
    return this->Read(buf, len, true);
}

unsigned int Moho::CNetTCPBuf::VirtReadNonBlocking(char *buf, unsigned int len) {
    return this->Read(buf, len, false);
}

bool Moho::CNetTCPBuf::VirtAtEnd() {
    if (this->socket = INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    return this->failed;
}

void Moho::CNetTCPBuf::VirtWrite(const char *buf, unsigned int len) {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    if ((this->mode & gpg::Stream::ModeSend) == 0) {
        throw std::runtime_error{std::string{"output shutdown"}};
    }
    if (len) {
        this->Write(buf, len);
    }
}

void Moho::CNetTCPBuf::VirtFlush() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    int len = this->writeStart - this->writeHead;
    if (len)  {
        if (send(socket, &this->buffer[2048], len, 0) == SOCKET_ERROR) {
            gpg::Logf("CNetTCPBuf::Flush(): send() failed: %s", Moho::NET_GetWinsockErrorString());
        }
        this->writeStart = &this->buffer[2048];
    }
}
void Moho::CNetTCPBuf::VirtClose(gpg::Stream::Mode mode) {
    if (this->socket != INVALID_SOCKET) {
        if ((this->mode & mode & gpg::Stream::ModeSend) != 0) {
            this->VirtFlush();
            shutdown(this->socket, SD_SEND);
            this->mode &=~ gpg::Stream::ModeSend;
            this->dataEnd = 0;
            this->writeStart = 0;
            this->writeHead = 0;
        }
        if ((this->mode & mode & gpg::Stream::ModeReceive) != 0) {
            shutdown(this->socket, SD_RECEIVE);
            this->mode &=~ gpg::Stream::ModeReceive;
            this->end = 0;
            this->readHead = 0;
            this->start = 0;
        }
        if (this->mode != gpg::Stream::ModeNone) {
            closesocket(this->socket);
            this->socket = INVALID_SOCKET;
        }
    }
}

u_short Moho::CNetTCPBuf::GetPort() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    getsockname(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}

u_long Moho::CNetTCPBuf::GetPeerAddr() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    getpeername(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohl(name.sin_addr.S_un.S_addr);
}

u_short Moho::CNetTCPBuf::GetPeerPort() {
    if (this->socket == INVALID_SOCKET) {
        throw std::runtime_error{std::string{"socket closed"}};
    }
    sockaddr_in name;
    int namelen = sizeof(name);
    getpeername(this->socket, (SOCKADDR *) &name, &namelen);
    return ntohs(name.sin_port);
}


Moho::INetTCPSocket *Moho::NET_TCPConnect(u_long addr, u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_TCPConnect(%s:%d): socket() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(addr);
    if (connect(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_TCPConnect(%s:%d): connect() failed: %s",
            Moho::NET_GetHostName(addr).c_str(), port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPBuf{sock};
}

Moho::INetTCPServer *Moho::NET_CreateTCPServer(u_long addr, u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_CreateTCPServer(%d): socket() failed: %s", port, Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(addr);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): bind() failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_CreateTCPServer(%d): listen() failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPServerImpl{sock};
}

Moho::INetConnector *Moho::NET_MakeTCPConnector(u_short port) {
    if (! Moho::NET_Init()) {
        return nullptr;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        gpg::Logf("NET_MakeConnector: socket() failed: %s", Moho::NET_GetWinsockErrorString());
        return nullptr;
    }
    unsigned long argp = 1;
    if (ioctlsocket(sock, FIONBIO, &argp) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: ioctlsocket(FIONBIO) failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.S_un.S_addr = htonl(0);
    if (bind(sock, (SOCKADDR *) &name, sizeof(name)) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: bind(%d) failed: %s", port, Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        gpg::Logf("NET_MakeConnector: listen() failed: %s", Moho::NET_GetWinsockErrorString());
        closesocket(sock);
        return nullptr;
    }
    return new Moho::CNetTCPConnector{sock};
}
