
namespace gpg {

template<class T>
struct DatListItem
{
    DatListItem<T> *prev;
    DatListItem<T> *next;

    DatListItem() :
        prev{this},
        next{this}
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
        this->prev->next = this->next;
        this->next->prev = this->prev;
        this->next = this;
        this->prev = this;
    } // inline
    void InsertBefore(DatListItem<T> *that) {
        this->prev = that->prev;
        this->next = that;
        that->prev = this;
        this->prev->next = this;
    } // inline
};

}
