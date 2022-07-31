cmake_minimum_required(VERSION 2.8.12...3.22)
project(glad LANGUAGES C)

set(HEADER_FILES 
    include/glad/glad.h
    include/KHR/khrplatform.h
)
set(SOURCE_FILES 
    src/glad.c
)

add_library(glad ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(glad PUBLIC include)
add_library(glad::glad ALIAS glad)

