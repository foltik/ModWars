#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <algorithm>
#include <filesystem>

#include <sys/uio.h>


namespace fs = std::filesystem;

int pid;

template<typename T>
inline iovec iov(T* address) {
    return iovec {(void*)address, sizeof(T)};
}

template<typename T>
void write(uintptr_t address, T&& value) {
    process_vm_writev(pid, iov(&value), 1, iov<T>(address), 1, 0);
}

template<typename T>
void writeb(uintptr_t address, T* value, size_t n) {

}

template<typename T, typename A>
T read(A address) {
    T value;

    auto local = iov(&value);
    auto remote = iov((T*)address);

    if (process_vm_readv(pid, &local, 1, &remote, 1, 0) != -1)
        return value;
    else {
        std::stringstream stream;
        stream << "0x" << std::hex << address;
        throw std::runtime_error("Failed to read from " + stream.str());
    }

    return value;
}

template<typename T>
T* readb(uintptr_t address, T* value, size_t n) {
    T* buf = new T[n];
    for (int i = 0; i < n; i++)
        buf[i] = read<T>(pid, (T*)address + i);
    return buf;
}

/*
template<typename T>
struct address_t<T, typename enable_if<is_multi_ptr<T> >::type> {
    address_t<typename std::remove_pointer<T>::type> operator*() {
        return address_t(read<typename std::remove_pointer<T>::type>(addr));
    }
};

template<typename T>
struct address_t<T, std::negation<typename enable_if<is_multi_ptr<T> >::type>> {
    typename std::remove_pointer<T>::type operator*() {
        return read<typename std::remove_pointer<T>::type>(addr);
    }
};
*/

/*
template <typename T>
struct address_t<T, typename std::enable_if<std::is_floating_point<T>::value >::type*> {
    float operator*() {
        return 3.14;
    }
};
*/

int pidof(std::string_view target) {
    for (auto& file: fs::directory_iterator("/proc")) {
        auto name = file.path().filename().string();
        int pid = std::all_of(name.begin(), name.end(), ::isdigit) ? std::stoi(name) : 0;
        if (pid) {
            std::ifstream stream(file / "comm", std::ios::in);
            std::string name;
            stream >> name;
            stream.close();
            if (name == target)
                return pid;
        }
    }
    return 0;
}

uintptr_t image_base(int pid) {
    auto file = fs::path("/proc/" + std::to_string(pid) + "/maps");
    std::ifstream stream(file);
    std::string line;
    stream >> line;
    return std::stoull(line.substr(0, line.find('-')), 0, 16);
}

template<typename T>
struct is_multi_ptr
    : std::integral_constant<bool,
          std::is_pointer<T>::value &&
          std::is_pointer<std::remove_pointer_t<T> >::value > {};

template<typename T>
struct is_single_ptr
    : std::integral_constant<bool,
          std::is_pointer<T>::value &&
          !std::is_pointer<std::remove_pointer_t<T> >::value > {};

template<typename T, bool Base = false>
struct address;

template<typename T>
struct address<T, true> {
    T addr;

    address(T val) {
        addr = val;
    }
};

template<typename T>
struct address<T*, false> : public address<T*, true> {
    address(T* val) : address<T*, true>(val) {}
};

template <typename T>
struct address<T**, false> : public address<T**, true> {
};

int main(int argc, char* argv[]) {
    pid = pidof("defender");
    if (pid == 0) {
        std::cout << "[Attacker] Could not find defender" << std::endl;
        return 1;
    }

    std::cout << "[Attacker] Found defender with pid " << pid << std::endl;

    auto base = image_base(pid);

    std::cout << "[Attacker] Found defender base at " << std::hex << base << std::endl;

    //std::cout << std::hex << base + 0x18e70 << std::endl;
    //auto table_ptr = read<void**>(pid, base + 0x18e70);

    //address_t<void**> addr{base + 0x18e70};
    //auto addr = address_t(3.14f);
    //float asd = *addr;

    //auto newaddr = *addr;
    //address_t newaddr(*addr);
    //std::cout << std::hex << *addr << std::endl;

    address<int*> thing((int*)12345);

    return 0;
}

