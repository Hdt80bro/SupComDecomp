#include "gpgcore/containers/fastvector.h"

namespace Moho {

struct CMessage
{
    gpg::fastvector_n<char, 64> mBuff;
    int mPos;

    void SetSize(int size) {
        this->mBuff[1] = LOBYTE(size);
        this->mBuff[2] = HIBYTE(size);
    } // inline e.g. 0x0047BE62
    unsigned short GetSize() {
        // return *(unsigned short *)(&this->mBuff[1]);
        return MAKEWORD(this->mBuff[1], this->mBuff[2]);
    } // inline e.g. 0x0047BF4C
    bool HasReadLength() {
        return this->mPos >= 3;
    } // inline e.g. 0x0047BEE5
    char GetType() {
        return this->mBuff[0];
    } // inline e.g. 0x007BFB97
    void SetType(char type) {
        this->mBuff[0] = type;
    } // inline e.g. 0x004834E9

    CMessage(int size, char type); // 0x00483490
    int GetMessageSize(); // 0x0047BE90
    bool ReadMessage(gpg::Stream *stream); // 0x0047BD40
    bool Read(gpg::Stream *stream); // 0x0047BEE0
    unsigned int Append(char *ptr, int size); // 0x0047BDE0
};

}
