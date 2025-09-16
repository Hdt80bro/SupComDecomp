
namespace gpg {

template<class T, class U>
struct DListItem
{
    using type = T;
    using unk_t = U;
    using item_t = gpg::DListItem<type, unk_t>;

    item_t *mPrev;
    item_t *mNext;

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

template<class T, class U>
struct DList : gpg::DListItem<T, U>
{
    using type = T;
    using unk_t = U;
    using item_t = gpg::DListItem<type, unk_t>;

    struct iterator
    {
        item_t *pos;

        iterator(item_t *pos) : pos{pos} {}

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
        bool operator==(const iterator that&) {
            return this->pos == that.pos;
        }
        bool operator!=(const iterator that&) {
            return ! (*this == that);
        }
    };

    iterator begin() {
        return this->mNext;
    }
    iterator end() {
        return this;
    }
};

}
