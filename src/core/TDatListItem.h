
namespace Moho {

template<class T>
struct TDatListItem
{
    TDatListItem<T>* prev;
    TDatListItem<T>* next;

    T *Get() {
        return static_cast<T*>(this);
    } // inline
    ~TDatListItem() {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        this->next = this;
        this->prev = this;
    } // inline
};

};
