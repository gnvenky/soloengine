cmake_minimum_required(VERSION 3.16)
project(SlottedEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(storage)

add_executable(slotted_engine
    main.cpp
    Page.cpp
    BlockPool.cpp
)

