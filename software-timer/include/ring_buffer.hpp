#pragma once

#include <array>
#include <cstddef>

template<typename T, size_t Size>class RingBuffer {
private:
    std::array<T, Size> data;
    volatile int write_ind = 0;
    volatile int read_ind = 0;

public:
    bool push(T item) {
        if((write_ind+1)%Size==read_ind) {
            return false;
        }
        data[write_ind] = item;
        write_ind = (write_ind+1)%Size;
        return true;
    }

    [[nodiscard]]bool pop(T * item) {
        if(write_ind == read_ind) {
            return false;
        }
        *item = data[read_ind];
        read_ind = (read_ind +1)%Size;
        return true;
    }
    bool isEmpty() const {
        return write_ind == read_ind;
    }
};
