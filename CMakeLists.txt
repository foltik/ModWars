cmake_minimum_required(VERSION 3.5)
project(modwars VERSION 1.0.0 LANGUAGES CXX)

find_package(Threads REQUIRED)

add_executable(judge judge/main.cpp)
target_compile_features(judge PRIVATE cxx_std_17)

add_executable(attacker attacker/main.cpp)
target_compile_features(attacker PRIVATE cxx_std_17)
target_link_libraries(attacker stdc++fs)

add_executable(defender defender/main.cpp)
target_compile_features(defender PRIVATE cxx_std_17)
target_link_libraries(defender Threads::Threads)
