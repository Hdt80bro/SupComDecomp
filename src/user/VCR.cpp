#include "VCR.h"

std::auto_ptr<gpg::Stream> Moho::VCR_CreateReplay(const Moho::SWldSessionInfo *arg4, gpg::StrArg Str1) {
    const char **v3; // ebp
    std::string *start; // edi
    std::string *ReplayExt; // eax
    const char *v8; // ecx
    bool v9; // cf
    const char *Buf; // eax
    char *Ptr; // eax
    std::string *v12; // eax
    const char *v13; // edi
    std::string::_Bxty *p_Bx; // eax
    const char *v15; // ecx
    std::wstring *v16; // eax
    const WCHAR *v17; // eax
    bool v18; // bl
    std::string::_Bxty *v19; // eax
    std::string *v20; // eax
    std::string *CurrentProfileName; // eax
    std::string *v22; // eax
    std::string *v23; // eax
    const char *v24; // ecx
    std::wstring *v25; // eax
    const WCHAR *v26; // eax
    bool v27; // bl
    std::string::_Bxty *v28; // eax
    std::string *v29; // eax
    std::string *v30; // esi
    std::string *v31; // eax
    const char *v32; // esi
    char *v33; // eax
    std::string *v34; // eax
    std::string::_Bxty *v35; // edx
    gpg::Stream **v36; // eax
    gpg::Stream *v37; // esi
    std::string::_Bxty *v38; // eax
    std::string *v39; // eax
    char *writeStart; // eax
    int v41; // edx
    char *v42; // ecx
    unsigned int v43; // edi
    std::vector_string *v44; // edx
    char *v45; // ecx
    unsigned int v46; // eax
    int v47; // edx
    char *v48; // ecx
    unsigned int v49; // edi
    std::vector_string *v50; // edx
    char *v51; // ecx
    unsigned int v52; // eax
    const char *v53; // eax
    int v54; // ebx
    char *v55; // ecx
    int v56; // ebp
    const char *v57; // edi
    int v58; // eax
    const char *v59; // edi
    std::vector_string *v60; // edx
    char *v61; // ecx
    const char *v62; // ebx
    unsigned int v63; // eax
    char *v64; // ecx
    int v65; // edi
    char v66; // dl
    char *v67; // ecx
    int v68; // eax
    int v69; // ebx
    char *v70; // ecx
    char *v71; // ebp
    int v72; // edx
    char *v73; // ecx
    int v74; // eax
    const char *v75; // edi
    char *v76; // eax
    std::vector_string *v77; // eax
    char *v78; // edx
    unsigned int v79; // ecx
    Moho::SSyncFilter_Subobj1 *v80; // edi
    int i; // ebx
    char *v82; // ecx
    char *v83; // ecx
    char *v84; // ecx
    std::string *v85; // esi
    std::vector_string *p_ebx0; // [esp+54h] [ebp-110h] BYREF
    const char *v87[6]; // [esp+58h] [ebp-10Ch] BYREF
    int v88; // [esp+70h] [ebp-F4h] BYREF
    gpg::Stream *v89; // [esp+74h] [ebp-F0h] BYREF
    const char **v90; // [esp+78h] [ebp-ECh] BYREF
    std::vector_string **v91; // [esp+7Ch] [ebp-E8h] BYREF
    int v92; // [esp+80h] [ebp-E4h] BYREF
    int v93; // [esp+84h] [ebp-E0h]
    const char *v94; // [esp+88h] [ebp-DCh]
    std::string a2; // [esp+8Ch] [ebp-D8h] BYREF
    int v96; // [esp+A8h] [ebp-BCh]
    std::string v97; // [esp+ACh] [ebp-B8h] BYREF
    std::string v99; // [esp+D8h] [ebp-8Ch] BYREF
    std::string a1; // [esp+F4h] [ebp-70h] BYREF
    std::runtime_error arg1; // [esp+110h] [ebp-54h] BYREF
    std::string dest; // [esp+138h] [ebp-2Ch] BYREF

    v96 = 0;
    v3 = (const char **)(*((int (__thiscall **)(Moho::SFileStarCloser *))arg4->mLaunchInfo.closer->_ptr + 1))(arg4->mLaunchInfo.closer);
    v90 = v3;
    if (v3) {
        v97._Myres = 15;
        v97._Mysize = 0;
        v97._Bx._Buf[0] = 0;
        v103 = 2;
        v91 = (std::vector_string **)v87;
        std::vector<std::string> args{};
        if (Moho::CFG_GetArgOption("/savereplay", 1, &args)) {
            ReplayExt = Moho::USER_GetReplayExt(&a1);
            v87[0] = v8;
            v9 = ReplayExt->_Myres < 0x10;
            v91 = (std::vector_string **)v87;
            if ( v9 )
                Buf = ReplayExt->_Bx._Buf;
            else
                Buf = ReplayExt->_Bx._Ptr;
            v87[0] = Buf;
            v9 = start->_Myres < 0x10;
            p_ebx0 = (std::vector_string *)v87;
            v91 = &p_ebx0;
            if (v9)
                Ptr = (char *)start->_Bx._Buf;
            else
                Ptr = (char *)start->_Bx._Ptr;
            v12 = Moho::FILE_SuggestedExt(&a2, Ptr, v87[0]);
            std::string::assign(&v97, v12, 0, 0xFFFFFFFF);
            if (a2._Myres >= 0x10)
                operator delete((void *)a2._Bx._Ptr);
            a2._Myres = 15;
            a2._Mysize = 0;
            a2._Bx._Buf[0] = 0;
            if (a1._Myres >= 0x10)
                operator delete((void *)a1._Bx._Ptr);
        } else {
            v99._Myres = 15;
            v99._Mysize = 0;
            v99._Bx._Buf[0] = 0;
            v13 = Str1;
            LOBYTE(v103) = 5;
            if (! strchr(Str1, 47) && ! strchr(Str1, 92)) {
                Moho::USER_GetReplayDir(&a2);
                LOBYTE(v103) = 6;
                p_Bx = (std::string::_Bxty *)a2._Bx._Ptr;
                v87[0] = v15;
                v91 = (std::vector_string **)v87;
                if (a2._Myres < 0x10)
                    p_Bx = &a2._Bx;
                v16 = gpg::STR_Utf8ToWide((std::wstring *)&arg1, p_Bx->_Buf);
                if (v16->_Myres < 8)
                    v17 = v16->_Bx._Buf;
                else
                    v17 = v16->_Bx._Ptr;
                v18 = ! CreateDirectoryW(v17, 0) && GetLastError() != 183;
                v96 = 0;
                std::wstring::~wstring(&arg1);
                if (v18) {
                    v19 = (std::string::_Bxty *)a2._Bx._Ptr;
                    if (a2._Myres < 0x10)
                        v19 = &a2._Bx;
                    v20 = gpg::STR_Printf(&dest, "Unable to create directory %s", v19->_Buf);
                    std::runtime_error::runtime_error(&arg1, v20);
                    CxxThrowException(&arg1, &stru_EC210C);
                }
                CurrentProfileName = Moho::OPTIONS_GetCurrentProfileName(&dest);
                v22 = operator+<std::string,std::string>(&a1, &a2, CurrentProfileName);
                v23 = operator+<char,std::string>((std::string *)&arg1, v22, "\\");
                std::string::assign(&v99, v23, 0, 0xFFFFFFFF);
                std::string::~string(&arg1);
                std::string::~string(&a1);
                std::string::~string(&dest);
                v87[0] = v24;
                v91 = (std::vector_string **)v87;
                func_cstrSet(v87, &v99);
                v25 = gpg::STR_Utf8ToWide((std::wstring *)&dest, v87[0]);
                if (v25->_Myres < 8)
                    v26 = v25->_Bx._Buf;
                else
                    v26 = v25->_Bx._Ptr;
                v27 = !CreateDirectoryW(v26, 0) && GetLastError() != 183;
                v96 = 0;
                std::wstring::~wstring(&dest);
                if (v27) {
                    v28 = (std::string::_Bxty *)v99._Bx._Ptr;
                    if (v99._Myres < 0x10)
                        v28 = &v99._Bx;
                    v29 = gpg::STR_Printf(&dest, "Unable to create directory %s", v28->_Buf);
                    LOBYTE(v103) = 11;
                    std::runtime_error::runtime_error(&arg1, v29);
                    CxxThrowException(&arg1, &stru_EC210C);
                }
                LOBYTE(v103) = 5;
                std::string::~string(&a2);
                v13 = Str1;
            }
            v30 = Moho::USER_GetReplayExt((std::string *)&arg1);
            LOBYTE(v103) = 12;
            v31 = operator+<char,std::string>(&a1, &v99, v13);
            LOBYTE(v103) = 13;
            v9 = v30->_Myres < 0x10;
            v91 = (std::vector_string **)v87;
            if (v9)
                v32 = v30->_Bx._Buf;
            else
                v32 = v30->_Bx._Ptr;
            v87[0] = v32;
            v9 = v31->_Myres < 0x10;
            p_ebx0 = (std::vector_string *)v87;
            v91 = &p_ebx0;
            if (v9)
                v33 = (char *)v31->_Bx._Buf;
            else
                v33 = (char *)v31->_Bx._Ptr;
            v34 = Moho::FILE_SuggestedExt(&a2, v33, v87[0]);
            std::string::assign(&v97, v34, 0, 0xFFFFFFFF);
            if (a2._Myres >= 0x10)
                operator delete((void *)a2._Bx._Ptr);
            a2._Myres = 15;
            a2._Mysize = 0;
            a2._Bx._Buf[0] = 0;
            if (a1._Myres >= 0x10)
                operator delete((void *)a1._Bx._Ptr);
            a1._Myres = 15;
            a1._Mysize = 0;
            a1._Bx._Buf[0] = 0;
            if ( *((_DWORD *)&arg1._Str._Bx._Ptr + 2) >= 0x10u )
                operator delete((void *)arg1._m_what);
            if (v99._Myres >= 0x10)
                operator delete((void *)v99._Bx._Ptr);
            v3 = v90;
        }
        v35 = (std::string::_Bxty *)v97._Bx._Ptr;
        v87[0] = (const char *)2;
        v91 = (std::vector_string **)v87;
        if ( v97._Myres < 0x10 )
        v35 = &v97._Bx;
        v36 = (gpg::Stream **)sub_875770(&v91, v35->_Buf);
        v37 = *v36;
        *v36 = 0;
        v94 = (const char *)v37;
        if (v91)
            ((void (__thiscall *)(std::vector_string **, int))(*v91)->_Myfirstiter)(v91, 1);
        v38 = (std::string::_Bxty *)v97._Bx._Ptr;
        if (v37) {
            if (v97._Myres < 0x10)
                v38 = &v97._Bx;
            gpg::Logf("Saving replay to \"%s\"", v38->_Buf);
            v89 = v37;
            gpg::Stream::Write(gpg::STR_Printf("Supreme Commander v%1.2f.%4i", 1.5, 3764), (int)&v89);
            gpg::Stream::Write("\r\n", &v89);
            writeStart = v37->writeStart;
            if ((unsigned int)(v37->dataEnd - writeStart) < 0xD) {
                v37->vtable->VirtWrite(v37, "Replay v1.9\r\n", 13);
            } else {
                qmemcpy(writeStart, "Replay v1.9\r\n", 13);
                v37->writeStart += 13;
            }
            gpg::Stream::Write(&arg4->mMapname, (int)&v89);
            gpg::Stream::Write(asc_E49660, &v89);
            v41 = (int)v3[8];
            v92 = v41;
            v42 = v37->writeStart;
            if ((unsigned int)(v37->dataEnd - v42) < 4) {
                v37->vtable->VirtWrite(v37, (char *)&v92, 4);
            } else {
                *(_DWORD *)v42 = v41;
                v37->writeStart += 4;
            }
            v43 = (unsigned int)v3[8];
            if ((unsigned int)v3[9] < 0x10)
                v44 = (std::vector_string *)(v3 + 4);
            else
                v44 = (std::vector_string *)v3[4];
            v45 = v37->writeStart;
            v46 = v37->dataEnd - v45;
            v87[0] = v3[8];
            p_ebx0 = v44;
            if (v43 > v46) {
                v37->vtable->VirtWrite(v37, (char *)p_ebx0, (size_t)v87[0]);
            } else {
                memcpy(v45, p_ebx0, (size_t)v87[0]);
                v37->writeStart += v43;
            }
            v47 = (int)v3[15];
            v92 = v47;
            v48 = v37->writeStart;
            if ((unsigned int)(v37->dataEnd - v48) < 4) {
                v37->vtable->VirtWrite(v37, (char *)&v92, 4);
            } else {
                *(_DWORD *)v48 = v47;
                v37->writeStart += 4;
            }
            v49 = (unsigned int)v3[15];
            if ((unsigned int)v3[16] < 0x10)
                v50 = (std::vector_string *)(v3 + 11);
            else
                v50 = (std::vector_string *)v3[11];
            v51 = v37->writeStart;
            v52 = v37->dataEnd - v51;
            v87[0] = v3[15];
            p_ebx0 = v50;
            if (v49 > v52) {
                v37->vtable->VirtWrite(v37, (char *)p_ebx0, (size_t)v87[0]);
            } else {
                memcpy(v51, p_ebx0, (size_t)v87[0]);
                v37->writeStart += v49;
            }
            v53 = v3[22];
            if (v53)
                v54 = (v3[23] - v53) / 36;
            else
                v54 = 0;
            HIBYTE(v88) = v54;
            v55 = v37->writeStart;
            if (v37->dataEnd == v55) {
                v37->vtable->VirtWrite(v37, (char *)&v88 + 3, 1);
            } else {
                *v55 = v54;
                ++v37->writeStart;
            }
            if (v54) {
                v56 = 0;
                v93 = v54;
                do {
                    v57 = v90[22];
                    v58 = *(_DWORD *)&v57[v56 + 24];
                    v59 = &v57[v56];
                    if (*((_DWORD *)v59 + 7) < 0x10u)
                        v60 = (std::vector_string *)(v59 + 8);
                    else
                        v60 = (std::vector_string *)*((_DWORD *)v59 + 2);
                    v61 = v37->writeStart;
                    v62 = (const char *)(v58 + 1);
                    v63 = v37->dataEnd - v61;
                    v87[0] = v62;
                    p_ebx0 = v60;
                    if ((unsigned int)v62 > v63) {
                        v37->vtable->VirtWrite(v37, (char *)p_ebx0, (size_t)v87[0]);
                    } else {
                        memcpy(v61, p_ebx0, (size_t)v87[0]);
                        v37->writeStart += (unsigned int)v62;
                    }
                    v64 = v37->writeStart;
                    if ((unsigned int)(v37->dataEnd - v64) < 4) {
                        v37->vtable->VirtWrite(v37, (char *)v59 + 32, 4);
                    } else {
                        *(_DWORD *)v64 = *((_DWORD *)v59 + 8);
                        v37->writeStart += 4;
                    }
                    v56 += 36;
                    --v93;
                } while ( v93 );
            }
            v65 = (int)v90;
            v66 = *((_BYTE *)v90 + 136);
            HIBYTE(v88) = v66;
            v67 = v37->writeStart;
            if (v37->dataEnd == v67) {
                v37->vtable->VirtWrite(v37, (char *)&v88 + 3, 1);
            } else {
                *v67 = v66;
                ++v37->writeStart;
            }
            v68 = *(_DWORD *)(v65 + 148);
            if (v68)
                v69 = (*(_DWORD *)(v65 + 152) - v68) / 28;
            else
                v69 = 0;
            HIBYTE(v88) = v69;
            v70 = v37->writeStart;
            if (v37->dataEnd == v70) {
                v37->vtable->VirtWrite(v37, (char *)&v88 + 3, 1);
            } else {
                *v70 = v69;
                ++v37->writeStart;
            }
            v71 = (char *)v65;
            if (v69) {
                v93 = 0;
                v89 = 0;
                v92 = v69;
                do {
                    v72 = *(int *)((char *)&v89->writeStart + *((_DWORD *)v71 + 37));
                    v90 = (const char **)v72;
                    v73 = v37->writeStart;
                    if ((unsigned int)(v37->dataEnd - v73) < 4) {
                        v37->vtable->VirtWrite(v37, (char *)&v90, 4);
                    } else {
                        *(_DWORD *)v73 = v72;
                        v37->writeStart += 4;
                    }
                    v74 = *((_DWORD *)v71 + 37);
                    v75 = *(char **)((char *)&v89->writeStart + v74);
                    v76 = (char *)v89 + v74;
                    if (*((_DWORD *)v76 + 6) < 0x10u)
                        v77 = (std::vector_string *)(v76 + 4);
                    else
                        v77 = (std::vector_string *)*((_DWORD *)v76 + 1);
                    v78 = v37->writeStart;
                    v79 = v37->dataEnd - v78;
                    v87[0] = v75;
                    p_ebx0 = v77;
                    if ((unsigned int)v75 > v79) {
                        v37->vtable->VirtWrite(v37, (char *)p_ebx0, (size_t)v87[0]);
                    } else {
                        memcpy(v78, p_ebx0, (size_t)v87[0]);
                        v37->writeStart += (unsigned int)v75;
                    }
                    v80 = (Moho::SSyncFilter_Subobj1 *)(v93 + *((_DWORD *)v71 + 18));
                    for (i = sub_4017B0(-1, v80);
                         i != 32 * (v80->v0 + v80->vec._M_finish - v80->vec._M_start);
                         i = sub_4017B0(i, v80)
                    ) {
                        HIBYTE(v88) = i;
                        v82 = v37->writeStart;
                        if (v37->dataEnd == v82) {
                            v37->vtable->VirtWrite(v37, (char *)&v88 + 3, 1);
                        } else {
                            *v82 = i;
                            ++v37->writeStart;
                        }
                    }
                    HIBYTE(v88) = -1;
                    v83 = v37->writeStart;
                    if (v37->dataEnd == v83) {
                        v37->vtable->VirtWrite(v37, (char *)&v88 + 3, 1);
                    } else {
                        *v83 = -1;
                        ++v37->writeStart;
                    }
                    ++v89;
                    v93 += 32;
                    --v92;
                } while (v92);
            }
            v84 = v37->writeStart;
            if ((unsigned int)(v37->dataEnd - v84) < 4) {
                v37->vtable->VirtWrite(v37, v71 + 160, 4);
            } else {
                *(_DWORD *)v84 = *((_DWORD *)v71 + 40);
                v37->writeStart += 4;
            }
            *arg0 = v37;
            v85 = ebx0.start;
            if (ebx0.start) {
                v87[0] = v94;
                p_ebx0 = &ebx0;
                func_DestroyStringsRange(ebx0.start, ebx0.finish);
                operator delete(v85);
            }
            if (v97._Myres >= 0x10)
                operator delete((void *)v97._Bx._Ptr);
            return arg0;
        } else {
            if (v97._Myres < 0x10)
                v38 = &v97._Bx;
            gpg::Logf("Can't create replay file \"%s\"", v38->_Buf);
            v94 = 0;
            *arg0 = 0;
            std::vector_string::~vector_string(&ebx0);
            if (v97._Myres >= 0x10)
                operator delete((void *)v97._Bx._Ptr);
            return arg0;
        }
    } else {
        *arg0 = 0;
        return arg0;
    }
}

Moho::SWldSessionInfo **__cdecl Moho::VCR_SetupReplaySession(Moho::SWldSessionInfo **arg0, const char *filename) {
    gpg::Stream **v2; // eax
    gpg::Stream *v3; // ecx
    Moho::SWldSessionInfo **result; // eax
    std::string::_Bxty *Ptr; // eax
    bool v6; // cf
    Moho::IArmy **v7; // esi
    Moho::LaunchInfoNew *v8; // eax MAPDST
    size_t Mysize; // eax
    int port; // edi
    const char *v12; // ecx
    size_t v13; // eax
    int v14; // edi
    std::string *p_scenarioInfo; // ecx
    std::string::_Bxty *p_Bx; // ecx
    int v17; // edi
    unsigned int v18; // esi
    char *v19; // ebp
    std::string *v20; // eax
    int v21; // esi
    int v22; // edi
    std::string::_Bxty *v23; // ecx
    _DWORD *v24; // ebp
    unsigned int i; // esi
    Moho::SWldSessionInfo *sesInfo; // eax MAPDST
    Moho::CClientManagerImpl *ClientManager; // eax
    Moho::CClientManagerImpl *mClientManager; // ecx
    Moho::CClientManagerImpl *v30; // esi
    Moho::CClientManagerImpl *v31; // esi
    int v32; // ecx
    LuaPlus::LuaState *LuaState; // eax
    std::string *v34; // eax
    int v35; // ecx
    std::string::_Bxty *v36; // eax
    int *v37; // eax
    bool v38; // zf
    _DWORD v39[4]; // [esp-10h] [ebp-114h] BYREF
    size_t v40[2]; // [esp+0h] [ebp-104h]
    int v41; // [esp+8h] [ebp-FCh] BYREF
    int p_a2; // [esp+Ch] [ebp-F8h]
    int v43; // [esp+10h] [ebp-F4h] BYREF
    int v44; // [esp+14h] [ebp-F0h]
    int v45; // [esp+18h] [ebp-ECh]
    const char *v46; // [esp+1Ch] [ebp-E8h] BYREF
    int v47; // [esp+24h] [ebp-E0h] BYREF
    gpg::BinaryReader v48; // [esp+28h] [ebp-DCh] BYREF
    gpg::Stream *v49; // [esp+38h] [ebp-CCh] BYREF
    int v50; // [esp+3Ch] [ebp-C8h]
    std::string a2; // [esp+40h] [ebp-C4h] BYREF
    Moho::LaunchInfoNew *v52; // [esp+5Ch] [ebp-A8h] BYREF
    std::string dest; // [esp+60h] [ebp-A4h] BYREF
    _DWORD a1[2]; // [esp+7Ch] [ebp-88h] BYREF
    int *v55; // [esp+84h] [ebp-80h]
    int *v56; // [esp+88h] [ebp-7Ch]
    char *v57; // [esp+8Ch] [ebp-78h]
    int *v58; // [esp+90h] [ebp-74h]
    int v59[2]; // [esp+94h] [ebp-70h] BYREF
    char v60; // [esp+9Ch] [ebp-68h] BYREF
    void (__thiscall ***v61)(_DWORD, int); // [esp+A0h] [ebp-64h] BYREF
    std::string v62; // [esp+A4h] [ebp-60h] BYREF
    int buf[3]; // [esp+C0h] [ebp-44h] BYREF
    __int16 v64; // [esp+CCh] [ebp-38h]
    std::string str1; // [esp+D0h] [ebp-34h] BYREF
    int v66; // [esp+E4h] [ebp-20h] SPLIT
    int a3[3]; // [esp+ECh] [ebp-18h] BYREF

    v50 = 0;
    v2 = (gpg::Stream **)sub_875770(&v61, filename);
    v3 = *v2;
    *v2 = 0;
    v49 = v3;
    v68 = 1;
    if (v61) {
        (**v61)(v61, 1);
        v3 = v49;
    }
    if (v3) {
        v48.stream = v3;
        gpg::STR_Printf(&dest, "Supreme Commander v%1.2f.%4i", 1.5, 3764);
        sub_4CCDD0(&v48, &v62);
        Ptr = (std::string::_Bxty *)dest._Bx._Ptr;
        if (dest._Myres < 0x10)
            Ptr = &dest._Bx;
        if (std::operator<<char,std::char_traits<char>,std::allocator<char>>(&v62, 0, v62._Mysize, Ptr->_Buf, dest._Mysize)) {
            v6 = v62._Myres < 0x10;
            v48.v2 = 0;
            *arg0 = 0;
            v50 = 1;
            if (! v6)
                operator delete((void *)v62._Bx._Ptr);
            v62._Myres = 15;
            v62._Mysize = 0;
            v62._Bx._Buf[0] = 0;
            if (dest._Myres >= 0x10)
                operator delete((void *)dest._Bx._Ptr);
            dest._Myres = 15;
            dest._Mysize = 0;
            dest._Bx._Buf[0] = 0;
            if (v49)
                v49->vtable->dtr(v49, 1);
            return arg0;
        } else {
            sub_4CCDD0(&v48, &a2);
            if (a2._Myres >= 0x10)
                operator delete((void *)a2._Bx._Ptr);
            memset(buf, 0, sizeof(buf));
            v64 = 0;
            gpg::BinaryReader::Read(&v48, buf, 13);
            sub_4CCDD0(&v48, &str1);
            sub_4CCDD0(&v48, &a2);
            if (a2._Myres >= 0x10)
                operator delete((void *)a2._Bx._Ptr);
            if (! strcmp((const char *)buf, "Replay v1.9\r\n")) {
                v8 = (Moho::LaunchInfoNew *)operator new(0xA4u);
                v48.v2 = v8;
                if (v8)
                    v8 = Moho::LaunchInfoNew::LaunchInfoNew(v8);
                else
                    v8 = 0;
                v52 = v8;
                gpg::BinaryReader::Read(&v48, &v48.port, 4);
                Mysize = v8->gameMods._Mysize;
                port = v48.port;
                if (v48.port > Mysize)
                    std::string::append(&v8->gameMods, v48.port - Mysize, 0);
                else
                    std::string::erase(&v8->gameMods, v48.port, 0xFFFFFFFF);
                if (v8->gameMods._Myres < 0x10)
                    v12 = v8->gameMods._Bx._Buf;
                else
                    v12 = v8->gameMods._Bx._Ptr;
                gpg::BinaryReader::Read(&v48, (void *)v12, port);
                gpg::BinaryReader::Read(&v48, &v48.port, 4);
                v13 = v8->scenarioInfo._Mysize;
                v14 = v48.port;
                p_scenarioInfo = &v8->scenarioInfo;
                if (v48.port > v13)
                    std::string::append(p_scenarioInfo, v48.port - v13, 0);
                else
                    std::string::erase(p_scenarioInfo, v48.port, 0xFFFFFFFF);
                if (v8->scenarioInfo._Myres < 0x10)
                    p_Bx = &v8->scenarioInfo._Bx;
                else
                    p_Bx = (std::string::_Bxty *)v8->scenarioInfo._Bx._Ptr;
                gpg::BinaryReader::Read(&v48, p_Bx, v14);
                a1[0] = 0;
                v55 = v59;
                v56 = v59;
                v57 = &v60;
                v58 = v59;
                LOBYTE(v68) = 7;
                gpg::BinaryReader::Read(&v48, (char *)&v47 + 3, 1);
                v17 = HIBYTE(v47);
                sub_876F60(HIBYTE(v47));
                v18 = 0;
                if (v17) {
                    v48.v3 = 0;
                    do {
                        v19 = (char *)v8->commandSources.start + v48.v3;
                        p_a2 = (int)&a2;
                        *(_DWORD *)v19 = v18;
                        v20 = sub_4CCDD0(&v48, (std::string *)p_a2);
                        std::string::assign((std::string *)(v19 + 4), v20, 0, 0xFFFFFFFF);
                        if (a2._Myres >= 0x10)
                            operator delete((void *)a2._Bx._Ptr);
                        gpg::BinaryReader::Read(&v48, &v48.port, 4);
                        p_a2 = (int)a3;
                        *((_DWORD *)v19 + 8) = v48.port;
                        func_Moho_SetAdd(a1, v18, p_a2);
                        v48.v3 += 36;
                        ++v18;
                    } while (v18 < HIBYTE(v47));
                }
                gpg::BinaryReader::Read(&v48, (char *)&v47 + 3, 1);
                v8->cheatsEnabled = HIBYTE(v47) != 0;
                gpg::BinaryReader::Read(&v48, (char *)&v47 + 3, 1);
                v21 = HIBYTE(v47);
                v48.v2 = v39;
                ((void (__stdcall *)(_DWORD, char, _DWORD, int *, int *, int *, int *))sub_543910)(
                    HIBYTE(v47),
                    0,
                    v39[1],
                    &v41,
                    &v41,
                    &v43,
                    &v41
                );
                if (v21) {
                    v48.v3 = 0;
                    v48.port = v21;
                    do {
                        gpg::BinaryReader::Read(&v48, &v48.v2, 4);
                        a2._Myres = 15;
                        a2._Mysize = 0;
                        a2._Bx._Buf[0] = 0;
                        v22 = v48.v2;
                        if (v48.v2) {
                            std::string::append(&a2, v48.v2, 0);
                            v23 = (std::string::_Bxty *)a2._Bx._Ptr;
                            if (a2._Myres < 0x10)
                                v23 = &a2._Bx;
                            gpg::BinaryReader::Read(&v48, v23, v22);
                        }
                        std::vector_string::push_back(&v8->strVec, &a2);
                        v24 = (int *)((char *)v8->vec2_start + v48.v3);
                        gpg::BinaryReader::Read(&v48, (char *)&v47 + 3, 1);
                        for (i = HIBYTE(v47); HIBYTE(v47) != 255; i = HIBYTE(v47)) {
                            func_Moho_SetAdd(v24, i, (int)a3);
                            gpg::BinaryReader::Read(&v48, (char *)&v47 + 3, 1);
                        }
                        if (a2._Myres >= 0x10)
                            operator delete((void *)a2._Bx._Ptr);
                        v48.v3 += 32;
                        --v48.port;
                    } while (v48.port);
                }
                gpg::BinaryReader::Read(&v48, &v48.v2, 4);
                v8->initSeed = v48.v2;
                p_a2 = 0x30;
                v8->commandSources.originalSource = -1;
                sesInfo = (Moho::SWldSessionInfo *)operator new(p_a2);
                if (sesInfo) {
                    sesInfo->mMapname._Myres = 15;
                    sesInfo->mMapname._Mysize = 0;
                    sesInfo->mMapname._Bx._Buf[0] = 0;
                    sesInfo->mLaunchInfo.closer = 0;
                    sesInfo->mLaunchInfo.count.pi_ = 0;
                    sesInfo->mIsBeingRecorded = 1;
                    sesInfo->mIsReplay = 0;
                    sesInfo->mIsMultiplayer = 0;
                    sesInfo->mClientManager = 0;
                    sesInfo->mSourceId = 255;
                } else {
                    sesInfo = 0;
                }
                v48.v2 = sesInfo;
                std::string::assign(&sesInfo->mMapname, &str1, 0, 0xFFFFFFFF);
                func_CopyLaunchInfoBasePtr((boost::shared_ptr_SFileStarCloser *)&v52, &sesInfo->mLaunchInfo);
                LOBYTE(p_a2) = 1;
                v41 = 0;
                v40[1] = 0;
                sesInfo->mIsBeingRecorded = 0;
                v40[0] = 2;
                sesInfo->mIsReplay = 1;
                ClientManager = Moho::CLIENT_CreateClientManager(v40[0], (Moho::INetConnector *)v40[1], v41, p_a2);
                mClientManager = sesInfo->mClientManager;
                v30 = ClientManager;
                if (ClientManager != mClientManager && mClientManager)
                    mClientManager->vtable->dtr(mClientManager, 1);
                p_a2 = (int)a1;
                sesInfo->mClientManager = v30;
                ((void (__thiscall *)(Moho::CClientManagerImpl *, gpg::Stream **, int, int, int, int, const char *))v30->vtable->CreateCReplayClient)(
                    v30,
                    &v49,
                    p_a2,
                    v43,
                    v44,
                    v45,
                    v46
                );
                v31 = sesInfo->mClientManager;
                v50 = (int)&v46;
                v45 = v32;
                v46 = "<LOC Engine0031>Local";
                LuaState = Moho::USER_GetLuaState();
                v34 = Moho::Loc(LuaState, (std::string *)(&a2._Bx._Ptr + 3), v46);
                v46 = (const char *)255;
                v45 = 1;
                v44 = 1;
                v6 = v34->_Myres < 0x10;
                v43 = v35;
                v50 = (int)&v43;
                if (v6)
                    v36 = &v34->_Bx;
                else
                    v36 = (std::string::_Bxty *)v34->_Bx._Ptr;
                v31->vtable->CreateCLocalClient(v31, v36->_Buf, v44, v45, (unsigned int)v46);
                if (a2._Myres >= 0x10)
                    operator delete((void *)a2._Bx._Ptr);
                v7 = (Moho::IArmy **)arg0;
                sesInfo->mSourceId = 255;
                v37 = v55;
                v38 = v55 == v58;
                *arg0 = sesInfo;
                v50 = 1;
                if (! v38) {
                    operator delete[](v37);
                    v37 = v58;
                    v55 = v58;
                    v57 = (char *)*v58;
                }
                v56 = v37;
                if (v52)
                    v52->vtable->dtr(v52, 1);
                if (str1._Myres >= 0x10)
                    operator delete((void *)str1._Bx._Ptr);
                str1._Myres = 15;
                v66 = 0;
                str1._Bx._Buf[0] = 0;
                if (v62._Myres >= 0x10)
                    operator delete((void *)v62._Bx._Ptr);
                v62._Myres = 15;
                v62._Mysize = 0;
                v62._Bx._Buf[0] = 0;
                if (dest._Myres >= 0x10)
                    operator delete((void *)dest._Bx._Ptr);
                dest._Myres = 15;
                dest._Mysize = 0;
                dest._Bx._Buf[0] = 0;
            } else {
                v7 = (Moho::IArmy **)arg0;
                v48.v2 = 0;
                *arg0 = 0;
                v50 = 1;
                std::string::~string(&str1);
                std::string::~string(&v62);
                std::string::~string(&dest);
            }
            if (v49)
                v49->vtable->dtr(v49, 1);
            return (Moho::SWldSessionInfo **)v7;
        }
    } else {
        result = arg0;
        *arg0 = 0;
    }
    return result;
}
