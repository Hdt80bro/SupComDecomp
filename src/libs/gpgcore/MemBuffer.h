#include <boost/shared_ptr.hpp>

namespace gpg {

template<class T>
struct MemBuffer
{
    boost::shared_ptr<T> mData;
    T *mBegin;
    T *mEnd;

    MemBuffer() :
        mData{},
        mBegin{nullptr},
        mEnd{nullptr}
    {}
    MemBuffer(const MemBuffer<T> &cpy) :
        mData{cpy.data},
        mBegin{cpy.begin},
        mEnd{cpy.end}
    {}
    MemBuffer(boost::shared_ptr<T> ptr, T *begin, T *end) :
        mData{ptr},
        mBegin{begin},
        mEnd{end}
    {}
    MemBuffer(boost::shared_ptr<T> ptr, unsigned int len) :
        mData{ptr},
        mBegin{ptr.data()},
        mEnd{ptr.data() + len}
    {}

    T *GetPtr(unsigned int start, unsigned int len) {
        T *begin = &this->mBegin[start];
        if (&begin[len] > this->mEnd) {
            throw std::range_error{std::string{"Out of bound access in MemBuffer<>::GetPtr()"}};
        }
        return begin;
    }
    gpg::MemBuffer<T> SubBuffer(unsigned int start, unsigned int len) {
        return gpg::MemBuffer<T>{this->mData, this->GetPtr(start, end), this->GetPtr(start + end, 0)};
    }
    void Reset() {
        this->mData.release();
        this->mBegin = nullptr;
        this->mEnd = nullptr;
    }
    unsigned int Size() {
        return (this->mEnd - this->mBegin) / sizeof(T);
    }
    gpg::MemBuffer<T> &operator=(gpg::MemBuffer<T> const &that) {
        this->mData = that.mData;
        this->mBegin = that.mBegin;
        this->mEnd = that.mEnd;
    }
    T *operator T *() {
        return *this->mBegin;
    }
};


/*
template<>
struct MemBuffer<char>
{
    MemBuffer(boost::shared_ptr<char> ptr, char *start, char *end); // 0x0094E0F0
    ~MemBuffer() = default; // 0x0042D1A0
    char *GetPtr(unsigned int start, unsigned int len); // 0x004313E0
};

template<>
struct MemBuffer<const char>
{
    gpg::MemBuffer<char> SubBuffer(unsigned int start, unsigned int len);
};
*/


gpg::MemBuffer<char> AllocMemBuffer(size_t size); // 0x0094E320

}
