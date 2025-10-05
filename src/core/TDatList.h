//#include "gpgcore/DList.h"

// it is curious that the class seems to be copied;
// both gpg::DList and Moho::TDatList are used, and
// one does not inherit the other as seen in `gpgcore.dll`

namespace Moho {

template<class T, class U>
struct TDatListItem
{
    using type = T;
    using unk_t = U;
    using item_t = Moho::TDatListItem<type, unk_t>;

    item_t *mPrev;
    item_t *mNext;

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
    void ListLinkBefore(item_t *that) {
        this->ListUnlink();
        this->mPrev = that->mPrev;
        this->mNext = that;
        that->mPrev = this;
        this->mPrev->mNext = this;
    }
    void ListLinkAfter(item_t *that) {
        this->ListUnlink();
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

template<class T, class U>
struct TDatList : Moho::TDatListItem<T, U>
{
    using type = T;
    using unk_t = U;
    using item_t = Moho::TDatListItem<type, unk_t>;
    
    struct iterator
    {
        item_t *pos;

        iterator(item_t *pos) : pos{pos} {}
        
        operator item_t*() {
            return this->pos;
        }
        iterator &operator++() {
            this->pos = this->pos->mNext;
            return *this;
        }
        type *operator*() {
            return this->pos.Get();
        }
        void *operator->() {
            return &**this;
        }
        bool operator==(const iterator &that) {
            return this->pos == that.pos;
        }
        bool operator!=(const iterator &that) {
            return ! (*this == that);
        }
    };

    TDatList() : Moho::TDatListItem{} {}

    iterator begin() {
        return this->mStart;
    }
    iterator end() {
        return this;
    }
    std::string{}.begin()
};

}
