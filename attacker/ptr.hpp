#pragma once

#include <iostream>

#include "rw.hpp"

template<class T>
struct Test {
    T operator[](int i) {
        return T();
    }
};
template<class T>
struct Test<T[]> {


}
// Ptr<T[]>

// T operator[](int)
// Ptr<T> operator[]

// Ptr<T>

template<typename T>
class _Ptr {
    template<class U>
    struct _Ptr_t {
        using type = U*;
    };

public:
    using ptr = typename _Ptr_t<T>::type;

    _Ptr() = default;
    _Ptr(ptr p) : p(p) {}

    ptr& _ptr() { return p; }
    ptr _ptr() const { return p; }

private:
    ptr p;
};

template<typename T>
class Ptr {
    _Ptr<T> t;

public:
    using ptr = typename _Ptr<T>::ptr;

    // Default
    Ptr() noexcept : t() {}
    explicit Ptr(ptr p) noexcept : t(p) {}

    Ptr& operator=(Ptr&& o) noexcept {
        std::cout << "move assign" << std::endl;
        return *this;
    }
};

template<typename T>
struct ptr_base {
    T addr;

    template<typename U>
    explicit ptr_base(U val) {
        addr = (T)val;
    }

    template<typename U>
    ptr_base<T>& operator=(U rhs) {
        write(addr, rhs);
        return *this;
    }
};

/*
template<typename T>
struct ptr;

template<typename T>
struct ptr<T*> : public ptr_base<T*> {
    using ptr_base<T*>::ptr_base;
    using ptr_base<T*>::operator=;
    T operator*() {
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
*/

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
