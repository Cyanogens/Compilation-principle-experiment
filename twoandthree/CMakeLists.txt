cmake_minimum_required(VERSION 3.17)
project(two)

set(CMAKE_CXX_STANDARD 14)

add_executable(two main.cpp
        baseclass/Base.cpp
        baseclass/Base.h

        two/baseclass/LL.cpp
        two/baseclass/LL.h
        two/part/Judgement.cpp
        two/part/CreatTable.cpp
        two/part/Eliminate.cpp
        two/part/main.h

        three/baseclass/LR.h
        three/baseclass/LR.cpp
        three/part/LivePrefixes.cpp
        three/part/Action.cpp
        three/part/Judgement.cpp
        three/part/main.h
        )