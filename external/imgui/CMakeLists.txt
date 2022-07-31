cmake_minimum_required(VERSION 2.8.12...3.22)
project(imgui DESCRIPTION "ImGui files" LANGUAGES CXX)

set(HEADER_FILES 
    imconfig.h
    imgui.h
    imgui_impl_glfw.h
    imgui_impl_opengl3.h
    imgui_impl_opengl3_loader.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)
set(SOURCE_FILES 
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)

add_library(imgui ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(imgui::imgui ALIAS imgui)