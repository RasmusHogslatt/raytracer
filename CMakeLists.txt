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
    include/MyMaterial.h
    include/Window.h
    include/GUI.h
    include/Renderer.h
    include/DebugRenderer.h
    include/PixelRenderer.h
    include/MySphere.h
    include/Parameters.h
    include/MyCamera.h
    include/Scene.h
    include/Point.h
    include/Film.h
    include/Ray.h
    include/Interaction.h
    include/Primitive.h
    include/shapes/Shape.h
    include/shapes/Sphere.h
    include/accelerators/Aggregate.h
    include/cameras/Camera.h
    include/cameras/PerspectiveCamera.h
    include/samplers/Sampler.h
    include/filters/Filter.h
    include/materials/Material.h
    include/lights/Light.h
    include/integrators/Integrator.h
)

set(SOURCE_FILES
    main.cpp
    source/App.cpp
    source/Actor.cpp
    source/Texture.cpp
    source/Object.cpp
    source/MyMaterial.cpp
    source/Window.cpp
    source/GUI.cpp
    source/Point.cpp
    source/Renderer.cpp
    source/DebugRenderer.cpp
    source/PixelRenderer.cpp
    source/MySphere.cpp
    source/MyCamera.cpp
    source/Scene.cpp
    source/Primitive.cpp
    source/Film.cpp
    source/Ray.cpp
    source/Interaction.cpp
    source/shapes/Shape.cpp
    source/shapes/Sphere.cpp
    source/accelerators/Aggregate.cpp
    source/cameras/Camera.cpp
    source/cameras/PerspectiveCamera.cpp
    source/samplers/Sampler.cpp
    source/filters/Filter.cpp
    source/materials/Material.cpp
    source/lights/Light.cpp
    source/integrators/Integrator.cpp
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
