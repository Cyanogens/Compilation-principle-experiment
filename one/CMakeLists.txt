cmake_minimum_required(VERSION 3.17)
project(one)

set(CMAKE_CXX_STANDARD 14)

add_executable(One main.cpp
        part/Thompson.cpp
        baseclass/BaseClass.cpp
        baseclass/BaseClass.h
        part/LexicalAnalysis.cpp
        part/DFA.cpp
        part/main.h)