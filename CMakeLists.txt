cmake_minimum_required(VERSION 3.22)

set(This chip8)

project(${This})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Wextra -pedantic -Werror )

enable_testing()

set(Headers
    include/screen.h
)

set(Sources
    src/chip8.c
    src/screen.c
)

find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})

#Add libraries
find_library(SDL2_LIBRARY NAME SDL2)
add_executable(chip8 ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)


target_link_libraries(${This} ${SDL2_LIBRARIES})

