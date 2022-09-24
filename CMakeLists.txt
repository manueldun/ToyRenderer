cmake_minimum_required(VERSION 3.8)

project(ToyRenderer)

add_subdirectory(Renderer)

add_executable(Renderer main.cpp)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif(MSVC)

target_include_directories(Renderer PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Renderer
)

target_link_libraries(Renderer RendererLib)

set(currentDir ${} PARENT_SCOPE)