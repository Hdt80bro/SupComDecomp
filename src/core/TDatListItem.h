//#include "gpgcore/DListItem.h"

namespace Moho {

template<class T>
struct TDatListItem
{
    using type = T;

    Moho::TDatListItem<type> *mPrev;
    Moho::TDatListItem<type> *mNext;

    TDatListItem() :
        mPrev{this},
        mNext{this}
    {}
    ~TDatListItem() {
        this->ListUnlink();
    }
    void ListUnlink() {
        this->mPrev->mNext = this->mNext;
        this->mNext->mPrev = this->mPrev;
        this->mNext = this;
        this->mPrev = this;
    }
    void ListLinkBefore(Moho::TDatListItem<type> *that) {
        this->mPrev = that->mPrev;
        this->mNext = that;
        that->mPrev = this;
        this->mPrev->mNext = this;
    }
    void ListLinkAfter(Moho::TDatListItem<type> *that) {
        this->mPrev = that;
        this->mNext = that->mNext;
        that->mNext->mPrev = this;
        this->mPrev->mNext = this;
    }
    bool ListEmpty() {
        return this->mNext == this;
    }
    type *ListGetNext() {
        return static_cast<type *>(this->mNext);
    }
    type *ListGetPrev() {
        return static_cast<type *>(this->mPrev);
    }
};

}
