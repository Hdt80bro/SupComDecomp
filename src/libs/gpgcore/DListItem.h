
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
    {} // inline
    ~DListItem() {
        this->Reset();
    } // inline
    void ListUnlink() {
        this->mPrev->mNext = this->mNext;
        this->mNext->mPrev = this->mPrev;
        this->mNext = this;
        this->mPrev = this;
    } // inline
    void ListLinkBefore(type *that) {
        this->ListUnlink();
        this->mPrev = that->mPrev;
        this->mNext = that;
        that->mPrev = this;
        this->mPrev->mNext = this;
    } // inline
    void ListLinkAfter(type *that) {
        this->ListUnlink();
        this->mPrev = that;
        this->mNext = that->mNext;
        that->mNext->mPrev = this;
        this->mPrev->mNext = this;
    } // inline
    bool ListIsUnlinked() {
        return this->mNext == this;
    } // inline

    
    type *Get() {
        return static_cast<type *>(this);
    } // inline
    bool HasNext() {
        return this->mPrev != this->mNext;
    } // inline
};

}
