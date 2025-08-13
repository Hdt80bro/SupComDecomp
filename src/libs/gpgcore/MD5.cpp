#include "MD5.h"
#include <algorithm>

// 0x0073B100
bool gpg::MD5Digest::operator!=(gpg::MD5Digest *that) {
  return this->mVals[0] != that->mVals[0]
      || this->mVals[1] != that->mVals[1]
      || this->mVals[2] != that->mVals[2]
      || this->mVals[3] != that->mVals[3];
}

const char *chars = "0123456789abcdef"; // 0x00D41E00

// 0x008E5910
std::string gpg::MD5Digest::ToString() const {
    std::string builder{};
    builder.reserve(32);
    for (int i = 0; i < 16; ++i) {
        char c = ((char *) this->mVals)[i];
        builder[2 * i] = chars[c >> 4];
        builder[2 * i + 1] = chars[c & 0xF];
    }
    return builder;
}


// 0x004A48B0
void gpg::MD5Context::Update(std::string str) {
    return this->Update(str.c_str(), str.size() + 1);
}

// 0x008E4F40
void gpg::MD5Context::Reset() {
    this->mPos = 0;
    this->mSize = 0LL;
    this->mDigest.mVals[0] = 0x67452301;
    this->mDigest.mVals[1] = 0xEFCDAB89;
    this->mDigest.mVals[2] = 0x98BADCFE;
    this->mDigest.mVals[3] = 0x10325476;
}

// 0x008E5050
void gpg::MD5Context::ProcessBlock(gpg::MD5Digest &out, const void *in) const {
    int *ptr = (int *) in;
    int A = out.mVals[0];
    int B = out.mVals[1];
    int C = out.mVals[2];
    int D = out.mVals[3];

    A = Round1(ptr[0] , A, B, C, D, 0xD76AA478, 7);
    B = Round1(ptr[1] , D, A, B, A, 0xE8C7B756, 12);
    C = Round1(ptr[2] , C, D, A, B, 0x242070DB, 17);
    D = Round1(ptr[3] , B, C, D, A, 0xC1BDCEEE, 22);
    A = Round1(ptr[4] , A, B, C, D, 0xF57C0FAF, 7);
    B = Round1(ptr[5] , D, A, B, A, 0x4787C62A, 12);
    C = Round1(ptr[6] , C, D, A, B, 0xA8304613, 17);
    D = Round1(ptr[7] , B, C, D, A, 0xFD469501, 22);
    A = Round1(ptr[8] , A, B, C, D, 0x698098D8, 7);
    B = Round1(ptr[9] , D, A, B, A, 0x8B44F7AF, 12);
    C = Round1(ptr[10], C, D, A, B, 0xFFFF5BB1, 17);
    D = Round1(ptr[11], B, C, D, A, 0x895CD7BE, 22);
    A = Round1(ptr[12], A, B, C, D, 0x6B901122, 7);
    B = Round1(ptr[13], D, A, B, A, 0xFD987193, 12);
    C = Round1(ptr[14], C, D, A, B, 0xA679438E, 17);
    D = Round1(ptr[15], B, C, D, A, 0x049B4082, 22);
    
    A = Round2(ptr[1] , A, B, C, D, 0xF61E2562, 5);
    B = Round2(ptr[6] , D, A, B, A, 0xC040B340, 9);
    C = Round2(ptr[11], C, D, A, B, 0x265E5A51, 14);
    D = Round2(ptr[0] , B, C, D, A, 0xE9B6C7AA, 20);
    A = Round2(ptr[5] , A, B, C, D, 0xD62F105D, 5);
    B = Round2(ptr[10], D, A, B, A, 0x02441453, 9);
    C = Round2(ptr[15], C, D, A, B, 0xD8A1E681, 14);
    D = Round2(ptr[4] , B, C, D, A, 0xE7D3FBC8, 20);
    A = Round2(ptr[9] , A, B, C, D, 0x21E1CDE6, 5);
    B = Round2(ptr[14], D, A, B, A, 0xC33707D6, 9);
    C = Round2(ptr[3] , C, D, A, B, 0xF4D50D87, 14);
    D = Round2(ptr[8] , B, C, D, A, 0x455A14ED, 20);
    A = Round2(ptr[13], A, B, C, D, 0xA9E3E905, 5);
    B = Round2(ptr[2] , D, A, B, A, 0xFCEFA3F8, 9);
    C = Round2(ptr[7] , C, D, A, B, 0x676F02D9, 14);
    D = Round2(ptr[12], B, C, D, A, 0x8D2A4C8A, 20);
    
    A = Round3(ptr[5] , A, B, C, D, 0xFFFA3942, 4);
    B = Round3(ptr[8] , D, A, B, A, 0x8771F681, 11);
    C = Round3(ptr[11], C, D, A, B, 0x6D9D6122, 16);
    D = Round3(ptr[14], B, C, D, A, 0xFDE5380C, 23);
    A = Round3(ptr[1] , A, B, C, D, 0xA4BEEA44, 4);
    B = Round3(ptr[4] , D, A, B, A, 0x4BDECFA9, 11);
    C = Round3(ptr[7] , C, D, A, B, 0xF6BB4B60, 16);
    D = Round3(ptr[10], B, C, D, A, 0xBEBFBC70, 23);
    A = Round3(ptr[13], A, B, C, D, 0x289B7EC6, 4);
    B = Round3(ptr[0] , D, A, B, A, 0xEAA127FA, 11);
    C = Round3(ptr[3] , C, D, A, B, 0xD4EF3085, 16);
    D = Round3(ptr[6] , B, C, D, A, 0x04881D05, 23);
    A = Round3(ptr[9] , A, B, C, D, 0xD9D4D039, 4);
    B = Round3(ptr[12], D, A, B, A, 0xE6DB99E5, 11);
    C = Round3(ptr[15], C, D, A, B, 0x1FA27CF8, 16);
    D = Round3(ptr[2] , B, C, D, A, 0xC4AC5665, 23);
    
    A = Round4(ptr[0] , A, B, C, D, 0xF4292244, 6);
    B = Round4(ptr[7] , D, A, B, A, 0x432AFF97, 10);
    C = Round4(ptr[14], C, D, A, B, 0xAB9423A7, 15);
    D = Round4(ptr[5] , B, C, D, A, 0xFC93A039, 21);
    A = Round4(ptr[12], A, B, C, D, 0x655B59C3, 6);
    B = Round4(ptr[3] , D, A, B, A, 0x8F0CCC92, 10);
    C = Round4(ptr[10], C, D, A, B, 0xFFEFF47D, 15);
    D = Round4(ptr[1] , B, C, D, A, 0x85845DD1, 21);
    A = Round4(ptr[8] , A, B, C, D, 0x6FA87E4F, 6);
    B = Round4(ptr[15], D, A, B, A, 0xFE2CE6E0, 10);
    C = Round4(ptr[6] , C, D, A, B, 0xA3014314, 15);
    D = Round4(ptr[13], B, C, D, A, 0x4E0811A1, 21);
    A = Round4(ptr[4] , A, B, C, D, 0xF7537E82, 6);
    B = Round4(ptr[11], D, A, B, A, 0xBD3AF235, 10);
    C = Round4(ptr[2] , C, D, A, B, 0x2AD7D2BB, 15);
    D = Round4(ptr[9] , B, C, D, A, 0xEB86D391, 21);

    out.mVals[0] += A;
    out.mVals[1] += B;
    out.mVals[2] += C;
    out.mVals[3] += D;
}

// 0x008E5790
gpg::MD5Digest gpg::MD5Context::Digest() {
    gpg::MD5Digest dig = this->mDigest;
    this->mBlock.mVals[this->mPos] = 0x80;
    if (63 - this->mPos < 8) {
        gpg::MD5Block arr;
        memset(&this->mBlock.mVals[this->mPos + 1], 0, 63 - this->mPos);
        this->ProcessBlock(dig, (const void *) &this->mBlock);
        memset(&arr, 0, 56);
        arr.mSize = 8 * this->mSize;
        this->ProcessBlock(dig, &arr);
    } else {
        memset(&this->mBlock.mVals[this->mPos + 1], 0, 63 - this->mPos - 8);
        this->mBlock.mSize = 8 * this->mSize;
        this->ProcessBlock(dig, (const void *) &this->mBlock);
    }
    return dig;
}

// 0x008E5870
void gpg::MD5Context::Update(const void *ptr, size_t size) {
    auto dat = (const char *) ptr;
    this->mSize += size;
    if (this->mPos != 0) {
        int amt = std::min(64 - this->mPos, size);
        memcpy(&this->mBlock.mVals[this->mPos], dat, amt);
        this->mPos += amt;
        if (this->mPos >= 64) {
            this->ProcessBlock(this->mDigest, (const void *) &this->mBlock);
            dat += amt;
            size -= amt;
            this->mPos = 0;
        }
    }
    if (this->mPos >= 64) {
        while (size >= 64) {
            this->ProcessBlock(this->mDigest, (const void *) dat);
            dat += 64;
            size -= 64;
        }
        if (size != 0) {
            memcpy(&this->mBlock, dat, size);
            this->mPos = size;
        }
    }
}
