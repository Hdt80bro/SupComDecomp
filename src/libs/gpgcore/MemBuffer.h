#include <boost/shared_ptr.hpp>

namespace gpg {

template<class T>
struct MemBuffer
{
    boost::shared_ptr<T> data;
    T *begin;
    T *end;

    MemBuffer() :
        data{},
        begin{nullptr},
        end{nullptr}
    {}
    MemBuffer(const MemBuffer<T> &cpy) :
        data{cpy.data},
        begin{cpy.begin},
        end{cpy.end}
    {}
    MemBuffer(boost::shared_ptr<T> ptr, T *begin, T *end) :
        data{ptr},
        begin{begin},
        end{end}
    {}
    MemBuffer(boost::shared_ptr<T> ptr, unsigned int len) :
        data{ptr},
        begin{ptr.data()},
        end{ptr.data() + len}
    {}

    T *GetPtr(unsigned int start, unsigned int len) {
        T *begin = &this->begin[start];
        if (&begin[len] > this->end) {
            throw std::range_error{std::string{"Out of bound access in MemBuffer<>::GetPtr()"}};
        }
        return begin;
    }
    gpg::MemBuffer<T> SubBuffer(unsigned int start, unsigned int len) {
        return gpg::MemBuffer<T>{this->data, this->GetPtr(start, end), this->GetPtr(start + end, 0)};
    }
    void Reset() {
        this->data.release();
        this->begin = nullptr;
        this->end = nullptr;
    }
    unsigned int Size() {
        return (this->end - this->begin) / sizeof(T);
    }
    gpg::MemBuffer<T> &operator=(gpg::MemBuffer<T> const &that) {
        this->data = that.data;
        this->begin = that.data;
        this->end = that.end;
    }
    T *operator T *() {
        return this->begin;
    }
};


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


gpg::MemBuffer<char> AllocMemBuffer(unsigned int size); // 0x0094E320

};