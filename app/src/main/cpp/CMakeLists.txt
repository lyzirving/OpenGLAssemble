cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR}/include/common)
include_directories(${CMAKE_SOURCE_DIR}/include/renderer)

aux_source_directory(${CMAKE_SOURCE_DIR}/renderer CPP_RENDERER)

add_library( # Sets the name of the library.
             lib-assembler
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             JniLoader.cpp
             ${CPP_RENDERER})

target_link_libraries( # Specifies the target library.
                       lib-assembler
                       # Links the target library to the log library
                       # included in the NDK.
                       log
                       EGL
                       GLESv2
                       GLESv3
                       jnigraphics)