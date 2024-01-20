cmake_minimum_required(VERSION 3.15.0)
project(cpplox VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_executable(cpplox 
    src/main.cpp
    src/Scanner.cpp
)
target_include_directories(cpplox PRIVATE deps/include)

if (MSVC) 
    # Warning level 4
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Werror -Wpedantic)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
