cmake_minimum_required(VERSION 2.8.12...3.22)
project(Raytracer VERSION 1.0.0 DESCRIPTION "Raytracer" LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(enable_warnings target)
target_compile_options(${target} PUBLIC 
$<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
$<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wconversion>
$<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wconversion>
$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wconversion>
)
endfunction()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
add_subdirectory(external)

set(HEADER_FILES
	include/App.h
    include/Actor.h
    include/Texture.h
    include/Object.h
    include/Material.h
    include/Window.h
    include/GUI.h
    include/Renderer.h
    include/DebugRenderer.h
    include/Parameters.h
)

set(SOURCE_FILES
    main.cpp
    source/App.cpp
    source/Actor.cpp
    source/Texture.cpp
    source/Object.cpp
    source/Material.cpp
    source/Window.cpp
    source/GUI.cpp
    source/Renderer.cpp
    source/DebugRenderer.cpp
)

# Add header with path to resources and add header to include directory
set(INCLUDE_RESOURCES_STRING ${CMAKE_CURRENT_LIST_DIR}/resources)
configure_file(
    ${CMAKE_SOURCE_DIR}/resources/ResourcePath.h.in
    ${CMAKE_BINARY_DIR}/generated/ResourcePath.h @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

add_executable(Raytracer ${SOURCE_FILES} ${HEADER_FILES})

enable_warnings(Raytracer)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Raytracer)
target_include_directories(Raytracer PRIVATE include ${GLM_INCLUDE_DIR})

## target_link_libraries takes care of setting the include
target_link_libraries(Raytracer PRIVATE OpenGL::GL glad::glad glfw stb::stb glm::glm imgui)
