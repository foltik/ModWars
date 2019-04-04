#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <algorithm>
#include <filesystem>

#include "ptr.hpp"

namespace fs = std::filesystem;

extern int pid;
int pid;

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


int main(int argc, char* argv[]) {
    pid = pidof("defender");
    if (pid == 0) {
        std::cout << "[Attacker] Could not find defender" << std::endl;
        return 1;
    }

    std::cout << "[Attacker] Found defender with pid " << pid << std::endl;

    auto base = image_base(pid);

    std::cout << "[Attacker] Found defender base at " << std::hex << base << std::endl;

    ptr<char****> table(base + 0x61e0);
    std::cout << table << std::endl;
    std::cout << *table << std::endl;
    std::cout << **table << std::endl;
    std::cout << ***table << std::endl;

    return 0;
}

