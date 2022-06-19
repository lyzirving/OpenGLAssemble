cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR}/common/include)
include_directories(${CMAKE_SOURCE_DIR}/renderer/include)
include_directories(${CMAKE_SOURCE_DIR}/helper/include)
include_directories(${CMAKE_SOURCE_DIR}/model/include)
include_directories(${CMAKE_SOURCE_DIR}/third/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/common CPP_COMMON)
aux_source_directory(${CMAKE_SOURCE_DIR}/renderer CPP_RENDERER)
aux_source_directory(${CMAKE_SOURCE_DIR}/helper CPP_HELPER)
aux_source_directory(${CMAKE_SOURCE_DIR}/model CPP_MODEL)

add_library(lib-assimp
            SHARED
            IMPORTED)

set_target_properties(lib-assimp# lib name
                      PROPERTIES IMPORTED_LOCATION# property
                      ${CMAKE_SOURCE_DIR}/third/libs/assimp/${ANDROID_ABI}/libassimp.so)

add_library( # Sets the name of the library.
             lib-assembler
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             JniLoader.cpp
             ${CPP_COMMON}
             ${CPP_RENDERER}
             ${CPP_HELPER}
             ${CPP_MODEL})

target_link_libraries( # Specifies the target library.
                       lib-assembler
                       # Links the target library to the log library
                       # included in the NDK.
                       android
                       log
                       EGL
                       GLESv2
                       GLESv3
                       jnigraphics
                       lib-assimp)