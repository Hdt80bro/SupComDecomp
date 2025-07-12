
namespace gpg {

    template<class T>
    struct fastvector
    {
        T *start;
        T *end;
        T *capacity;

        fastvector() {
            this->start = &this->capacity;
            this->end = &this->capacity;
            this->capacity = &this->capacity;
        }
        ~fastvector() {
            delete[](this->start);
        }

        size_t Size() const {
            return (this->end - this->start) / sizeof(T);
        }
        size_t Capacity() const {
            return (this-capacity - this->start) / sizeof(T);
        }

        T &operator[](int idx) {
            return &this->start[idx];
        }
    };

    template<class T, int N>
    struct fastvector_n : public fastvector<T>
    {
        T *originalVec;
        T inlineVec[N];

        fastvector_n() {
            this->start = &this->inlineVec[0];
            this->end = &this->inlineVec[0];
            this->capacity = &this->inelineVec[N];
            this->originalVec = &this->inlineVec;
        }
        ~fastvector_n() {
            if (this->start != this->originalVec) {
                delete[](this->start);
                this->start = this->originalVec;
            }
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
                T *newEnd = &this->start[newSize];
                while (this->end != newEnd) {
                    T *cur = this->end;
                    this->end = cur + 1;
                    if (cur) {
                        *cur = *fill;
                    }
                }
            } else {
                T *newEnd = &this->start[newSize];
                if (this->end != newEnd) {
                    this->end = newEnd;
                }
            }
        }

        void InsertAt(T *pos, T *insStart, T *insEnd) {
            size_t insSize = insEnd - insStart;
            size_t newSize = insSize + this->Size();
            if (newSize <= this->Capacity()) {
                if (&pos[insSize] <= this->end) {
                    int offset = &this->end[insSize] - pos;
                    this->end = (T *)memcpy(this->end, this->end, &this->end[insSize]);
                    if (offset > 0) { 
                        memmove_s(&end[-offset], offset, pos, offset); // ?
                    }
                    if (insSize > 0) {
                        memmove_s(pos, insSize, insStart, insSize);
                    }
                } else {
                    T *oldEnd = this->end;
                    int oldOffset = oldEnd - pos;
                    this->end = (T *)memcpy(oldEnd, insEnd, &insStart[oldEnd - pos]);
                    this->end = (T *)memcpy(this->end, oldEnd, pos);
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
            T *beginSplice = memcpy(newArr, pos, this->start);
            T *endSplice = memcpy(beginSplice, end, start);
            T *newEnd = (T *)memcpy(endSplice, this->end, pos);
            for (T *i = this->start; i != this->end; ++i) // ?
                ;
            if (this->start == this->originalVec) {
                *this->originalVec = this->capacity; // ?
            } else {
                delete[](this->start);
            }
            this->start = newArr;
            this->end = newEnd;
            this->capacity = &newArr[size];
        }

    };

    template<>
    struct fastvector_n<char, 64>
    {
        void InsertAt(char *, char *, char *); // 0x0047C590
        void Resize(unsigned int newSize, char *); // 0x0047C680
        void GrowInsert(int, char *, char *, char *); // 0x0047C910
    };

};