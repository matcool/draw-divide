cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(draw-rate)

add_library(draw-rate SHARED src/draw-rate.cpp)

include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:matcool/cocos-headers#f99a3b1")
# CPMAddPackage("gh:HJfod/gd.h#90f2110")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")

target_compile_definitions(draw-rate PRIVATE CC_DLL=)

target_include_directories(draw-rate PRIVATE
    ${minhook_SOURCE_DIR}/include
)

target_link_libraries(draw-rate mat-dash cocos2d minhook)

add_library(jump-key SHARED src/jump-key.cpp)

target_compile_definitions(jump-key PRIVATE CC_DLL=)

target_include_directories(jump-key PRIVATE
    ${minhook_SOURCE_DIR}/include
)

target_link_libraries(jump-key mat-dash cocos2d minhook)

