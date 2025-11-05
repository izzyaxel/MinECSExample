cmake_minimum_required(VERSION 4.1)

project(EngineTest VERSION 1.0.0 LANGUAGES CXX)
project(GameTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26) #C++20 works on GCC, idk about other compilers

add_library(EngineTest SHARED
            src/api/gameengine.hh
            src/gameengine.cc)
target_sources(EngineTest
               PUBLIC FILE_SET CXX_MODULES FILES
               src/api/Components.ixx
               src/api/Memory.ixx)

add_executable(GameTest src/main.cc)
include_directories("src/api")
target_link_libraries(GameTest EngineTest)
