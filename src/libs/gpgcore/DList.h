
namespace gpg {

template<class T, class U>
struct DListItem
{
    using type = T;
    using unk_t = U;
    using item_t = gpg::DListItem<type, unk_t>;

    item_t *mPrev;
    item_t *mNext;

    DListItem() : // inline
        mPrev{this},
        mNext{this}
    {}
    ~DListItem() { // inline 
        this->Reset();
    }
    void ListUnlink() { // inline
        this->mPrev->mNext = this->mNext;
        this->mNext->mPrev = this->mPrev;
        this->mNext = this;
        this->mPrev = this;
    }
    void ListLinkBefore(item_t *that) { // inline
        this->ListUnlink();
        this->mPrev = that->mPrev;
        this->mNext = that;
        that->mPrev = this;
        this->mPrev->mNext = this;
    }
    void ListLinkAfter(item_t *that) { // inline
        this->ListUnlink();
        this->mPrev = that;
        this->mNext = that->mNext;
        that->mNext->mPrev = this;
        this->mPrev->mNext = this;
    }
    bool ListIsUnlinked() { // inline
        return this->mNext == this;
    }

    
    type *Get() { // inline
        return static_cast<type *>(this);
    }
    bool HasNext() { // inline
        return this->mPrev != this->mNext;
    }
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
        bool operator==(const iterator &that) {
            return this->pos == that.pos;
        }
        bool operator!=(const iterator &that) {
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
