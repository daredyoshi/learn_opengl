cmake_minimum_required (VERSION 3.14)
project (HelloTriangle VERSION 1.0)

# This is so Ycm can find and complete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIR})
# find_package( Boost 1.40 COMPONENTS program_options REQUIRED )

add_executable(HelloTriangle
    src/stb_image.cpp
    src/ShaderProgram.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Main.cpp
    src/glad.c
)

include_directories(${CMAKE_SOURCE_DIR}/include)
# link_directories(include)

# use boost
# find_package(Boost REQUIRED regex date_time system filesystem thread graph)
# include_directories(${BOOST_INCLUDE_DIRS})

target_compile_features(HelloTriangle PUBLIC cxx_std_17)

target_link_libraries(HelloTriangle PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARIES})
# target_link_libraries(HelloTriangle PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS} ${Boost_LIBRARIES})

# add compiler warning flags just when building this project
# via the BUILD_INTERFACE generator expression
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(gcc_flags "-Wall;-Werror;-Wunused;-Wextra;-Wshadow;-Wshadow;-Wformat=2;-pedantic-errors;-std=c++17")
set(msvc_flags "-W3")

target_compile_options(HelloTriangle PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )

# add custom target to copy shader files across

add_custom_target(
    copy_shader_files
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
    COMMENT "Copying shader files" VERBATIM
    )

add_dependencies( HelloTriangle copy_shader_files )

# add custom target to copy texture files across

add_custom_target(
    copy_texture_files 
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/textures
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures
    COMMENT "Copying texture files" VERBATIM
    )

add_dependencies( HelloTriangle copy_texture_files )



