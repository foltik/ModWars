#pragma once

#include <cstddef>
#include <memory>
#include <sstream>
#include <cxxabi.h>
#include <sys/uio.h>

extern int pid;

template<class T>
std::unique_ptr<char> type_name() {
    char *name = abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr);
    return std::unique_ptr<char>(name);
}

struct Read {};
struct Write {};

template<class T>
std::string format_address(T addr) {
    std::stringstream ss;
    ss << "(" << type_name<T>().get() << ")" "0x" << std::hex << (uintptr_t)addr;
    return ss.str();
}

template<class T>
struct memory_error;

template<>
struct memory_error<Read> : public std::runtime_error {
    template<class U>
    memory_error(U addr) : std::runtime_error("Failed to read from " + format_address(addr)) {}
};

template<>
struct memory_error<Write> : public std::runtime_error {
    template<class U>
    memory_error(U addr) : std::runtime_error("Failed to write to " + format_address(addr)) {}
};

template<class T, class U>
inline iovec iov(U address, size_t n) {
    return iovec {(void*)address, sizeof(T) * n};
}

template<class T, class U>
void read(U address, size_t n, T* out) {
    auto local = iov<T>(out, n);
    auto remote = iov<T>(address, n);
    if (process_vm_readv(pid, &local, 1, &remote, 1, 0) == -1)
        throw memory_error<Read>(address);
}

template<class T, class U>
T* read(U address, size_t n) {
    T out[n];
    read(address, n, &out);
    return out;
}

template<class T, class U>
T read(U address) {
    T out;
    read(address, 1, &out);
    return out;
}

template<class T, class U>
void write(U address, size_t n, T* in) {
    auto local = iov<T>(in, n);
    auto remote = iov<T>(address, n);
    if (process_vm_writev(pid, &local, 1, &remote, 1, 0) == -1)
        throw memory_error<Write>(address);
}

template<class T, class U>
void write(U address, T& in) {
    write(address, 1, in);
}

template<class T, class U>
void write(U address, T&& xin) {
    std::decay_t<T> in = std::move(xin);
    write(address, 1, &in);
}
