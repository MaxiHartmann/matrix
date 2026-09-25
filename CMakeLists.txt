cmake_minimum_required(VERSION 3.20)

project(matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(matrix
    src/main.cpp
)

target_link_libraries(matrix
    PRIVATE
        glfw
        GLEW::GLEW
        OpenGL::GL
)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
