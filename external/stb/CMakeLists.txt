cmake_minimum_required(VERSION 2.8.12...3.22)
project(stb DESCRIPTION "STB Image library" LANGUAGES CXX)

set(HEADER_FILES 
    stb_image.h
    stb_image_write.h
)
set(SOURCE_FILES 
    stb_image.cpp
    stb_image_write.cpp
)

add_library(stb ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(stb::stb ALIAS stb)