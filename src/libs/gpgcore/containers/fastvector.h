
namespace gpg {

template<class T>
struct fastvector
{
    T *mStart;
    T *mEnd;
    T *mCapacity;

    fastvector() {
        this->mStart = &this->mCapacity;
        this->mEnd = &this->mCapacity;
        this->mCapacity = &this->mCapacity;
    }
    ~fastvector() {
        delete[](this->mStart);
    }

    size_t Size() const {
        return (this->mEnd - this->mStart) / sizeof(T);
    }
    size_t Capacity() const {
        return (this->mCapacity - this->mStart) / sizeof(T);
    }

    T &operator[](int idx) {
        return &this->mStart[idx];
    }
};

template<class T, int N>
struct fastvector_n : public fastvector<T>
{
    // also stores the original capacity at [0] when not in use
    // (i.e. the zeroeth index in the inline vec)
    T *mOriginalVec;
    T mInlineVec[N];

    fastvector_n() {
        this->mStart = &this->mInlineVec[0];
        this->mEnd = &this->mInlineVec[0];
        this->mCapacity = &this->mInlineVec[N];
        this->mOriginalVec = &this->mInlineVec;
    }
    ~fastvector_n() {
        if (this->mStart != this->mOriginalVec) {
            delete[](this->mStart);
            this->mStart = this->mOriginalVec;
        }
    }

    void Clear() {
        if (this->mStart != this->mOriginalVec) {
            delete[](this->mStart);
            this->mStart = this->mOriginalVec;
            this->mCapacity = *this->mOriginalVec;
        }
        this->mEnd = this->mStart;
    }

    void Grow(unsigned int newSize) {
        if (newSize > this->Capacity()) {
            this->GrowInsert(newSize, this->start, this->start, this->start);
        }
    }

    void Resize(unsigned int newSize, T *fill) {
        if (newSize > this->Size()) {
            if (newSize > this->Capacity()) {
                this->GrowInsert(newSize, this->start, this->start, this->start);
            }
            T *newEnd = &this->mStart[newSize];
            while (this->mEnd != newEnd) {
                T *cur = this->mEnd;
                this->mEnd = cur + 1;
                if (cur != nullptr) {
                    *cur = *fill;
                }
            }
        } else {
            T *newEnd = &this->mStart[newSize];
            if (this->end != newEnd) {
                this->end = newEnd;
            }
        }
    }

    void InsertAt(T *pos, T *insStart, T *insEnd) {
        size_t insSize = insEnd - insStart;
        size_t newSize = insSize + this->Size();
        if (newSize <= this->Capacity()) {
            if (&pos[insSize] <= this->mEnd) {
                int offset = &this->mEnd[insSize] - pos;
                this->mEnd = (T *)memcpy(this->mEnd, this->mEnd, &this->mEnd[insSize]);
                if (offset > 0) { 
                    memmove_s(&end[-offset], offset, pos, offset); // ?
                }
                if (insSize > 0) {
                    memmove_s(pos, insSize, insStart, insSize);
                }
            } else {
                T *oldEnd = this->mEnd;
                int oldOffset = oldEnd - pos;
                this->mEnd = (T *) memcpy(oldEnd, insEnd, &insStart[oldEnd - pos]);
                this->mEnd = (T *) memcpy(this->mEnd, oldEnd, pos);
                if (oldOffset > 0) {
                    memmove_s(&oldEnd[-oldOffset], oldOffset, insStart, oldOffset);
                }
            }
        } else {
            size_t growth = 2 * this->Capacity();
            if (newSize < growth) {
                newSize = growth;
            }
            this->GrowInsert(newSize, pos, insStart, insEnd);
        }
    }

    void GrowInsert(int size, T *pos, T *start, T *end) {
        T *newArr = new T[size];
        T *beginSplice = memcpy(newArr, pos, this->mStart);
        T *endSplice = memcpy(beginSplice, end, start);
        T *newEnd = (T *)memcpy(endSplice, this->mEnd, pos);
        for (T *i = this->mStart; i != this->mEnd; ++i) // ?
            ;
        if (this->mStart == this->mOriginalVec) {
            *this->mOriginalVec = this->mCapacity;
        } else {
            delete[](this->mStart);
        }
        this->mStart = newArr;
        this->mEnd = newEnd;
        this->mCapacity = &newArr[size];
    }
    
    void Append(T &o) {
        if (this->mEnd == this->mCapacity) {
            this->InsertAt(this->mEnd, &o, &o + 1);
        } else {
            if (this->mEnd != nullptr) {
                *this->mEnd = o;
            }
            ++this->mEnd;
        }
    }

};

/*
template<>
struct fastvector_n<char, 64>
{
    void InsertAt(char *, char *, char *); // 0x0047C590
    void Resize(unsigned int newSize, char *); // 0x0047C680
    void GrowInsert(int, char *, char *, char *); // 0x0047C910
};
*/

}
