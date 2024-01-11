cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(epic-mods)

include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:matcool/cocos-headers#f99a3b1")
# CPMAddPackage("gh:HJfod/gd.h#90f2110")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")

add_library(minhook_include INTERFACE)
target_include_directories(minhook_include INTERFACE ${minhook_SOURCE_DIR}/include)
target_link_libraries(minhook_include INTERFACE minhook)

add_compile_definitions(CC_DLL=)

add_library(draw-rate SHARED src/draw-rate.cpp)

target_link_libraries(draw-rate mat-dash cocos2d minhook_include)

add_library(jump-key SHARED src/jump-key.cpp)

target_link_libraries(jump-key mat-dash cocos2d minhook_include)

