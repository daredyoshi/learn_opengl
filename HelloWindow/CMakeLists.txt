cmake_minimum_required (VERSION 3.14)
project (HelloWindow VERSION 1.0)

# This is so Ycm can find and complete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)


add_executable(HelloWindow
    include/glad/glad.h
    include/KHR/khrplatform.h

    src/Main.cpp
    src/glad.c
)

include_directories(include)
target_compile_features(HelloWindow PUBLIC cxx_std_17)

target_link_libraries(HelloWindow PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS})

# add compiler warning flags just when building this project
# via the BUILD_INTERFACE generator expression
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(gcc_flags "-Wall;-Werror;-Wunused;-Wextra;-Wshadow;-Wshadow;-Wformat=2;-pedantic-errors;-std=c++17")
set(msvc_flags "-W3")

target_compile_options(HelloWindow PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )
