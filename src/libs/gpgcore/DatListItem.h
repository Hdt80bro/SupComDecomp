
namespace gpg {

template<class T>
struct DatListItem
{
    DatListItem<T> *mPrev;
    DatListItem<T> *mNext;

    DatListItem() :
        mPrev{this},
        mNext{this}
    {
        this->Reset(); // usually
    } // inline
    ~DatListItem() {
        this->Reset();
    } // inline
    T *Get() {
        return static_cast<T*>(this);
    } // inline
    void Reset() {
        this->mPrev->mNext = this->mNext;
        this->mNext->mPrev = this->mPrev;
        this->mNext = this;
        this->mPrev = this;
    } // inline
    void InsertBefore(DatListItem<T> *that) {
        this->mPrev = that->mPrev;
        this->mNext = that;
        that->mPrev = this;
        this->mPrev->mNext = this;
    } // inline
};

}
