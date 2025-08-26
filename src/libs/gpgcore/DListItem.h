
namespace gpg {

template<class T>
struct DListItem
{
    using type = T;

    gpg::DListItem<type> *mPrev;
    gpg::DListItem<type> *mNext;

    DListItem() :
        mPrev{this},
        mNext{this}
    {
        this->Reset(); // usually
    } // inline
    ~DListItem() {
        this->Reset();
    } // inline
    type *Get() {
        return static_cast<type *>(this);
    } // inline
    void Reset() {
        this->mPrev->mNext = this->mNext;
        this->mNext->mPrev = this->mPrev;
        this->mNext = this;
        this->mPrev = this;
    } // inline
    void InsertBefore(gpg::DListItem<type> *that) {
        this->mPrev = that->mPrev;
        this->mNext = that;
        that->mPrev = this;
        this->mPrev->mNext = this;
    } // inline
    bool Empty() {
        return this->mNext == this;
    } // inline
    bool HasNext() {
        return this->mPrev != this->mNext;
    } // inline
};

}
