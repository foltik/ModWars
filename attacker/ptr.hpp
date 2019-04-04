#pragma once

#include <iostream>

#include "rw.hpp"

template<typename T>
struct ptr_base {
    T addr;

    template<typename U>
    explicit ptr_base(U val) {
        addr = (T)val;
    }

    template<typename U>
    ptr_base<T>& operator=(U rhs) {
        std::cout << "write" << std::endl;
        write(addr, rhs);
        return *this;
    }
};

template<typename T>
struct ptr;

template<typename T>
struct ptr<T*> : public ptr_base<T*> {
    using ptr_base<T*>::ptr_base;
    using ptr_base<T*>::operator=;
    T operator*() {
        std::cout << "*" << this->addr << std::endl;
        return read<T>(this->addr);
    }
};

template <typename T>
struct ptr<T**> : ptr_base<T**> {
    using ptr_base<T**>::ptr_base;
    using ptr_base<T**>::operator=;
    ptr<T*> operator*() {
        return ptr<T*>(read<T*>(this->addr));
    }
};

template<typename T>
std::ostream &operator<<(std::ostream &stream, ptr<T> const &ptr) {
    stream << format_address(ptr.addr);
    return stream;
}

// TODO: fix this
/*
template<typename T>
std::ostream &operator<<(std::ostream &stream, ptr<char*> const &ptr) {
    std::cout << "write char" << std::endl;
    //char* buf = read<char>(ptr.addr, 64);
    return stream << "xd";
    return stream;
}
*/
