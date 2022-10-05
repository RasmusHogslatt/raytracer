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
	include/core/App.h
    include/core/Texture.h
    include/core/Ray.h
    include/core/Scene.h
    include/core/Primitive.h
    include/shapes/Shape.h
    include/shapes/Sphere.h
    include/shapes/Polygon.h
    include/shapes/Triangle.h
    include/shapes/Rectangle.h
    include/shapes/Tetrahedron.h
    include/cameras/Camera.h
    include/cameras/PerspectiveCamera.h
    include/cameras/OrthographicCamera.h
    include/samplers/Sampler.h
    include/samplers/CenterSampler.h
    include/samplers/RandomSampler.h
    include/materials/Material.h
    include/lights/Light.h
    include/lights/SphereLight.h
    include/lights/RectangleLight.h
    include/integrators/Integrator.h
    include/integrators/MonteCarloIntegrator.h
)

set(SOURCE_FILES
    main.cpp
    source/core/App.cpp
    source/core/Texture.cpp
    source/core/Ray.cpp
    source/core/Scene.cpp
    source/core/Primitive.cpp
    source/shapes/Shape.cpp
    source/shapes/Sphere.cpp
    source/shapes/Polygon.cpp
    source/shapes/Triangle.cpp
    source/shapes/Rectangle.cpp
    source/shapes/Tetrahedron.cpp
    source/cameras/Camera.cpp
    source/cameras/PerspectiveCamera.cpp
    source/cameras/OrthographicCamera.cpp
    source/samplers/Sampler.cpp
    source/samplers/CenterSampler.cpp
    source/samplers/RandomSampler.cpp
    source/materials/Material.cpp
    source/lights/Light.cpp
    source/lights/SphereLight.cpp
    source/lights/RectangleLight.cpp
    source/integrators/Integrator.cpp
    source/integrators/MonteCarloIntegrator.cpp
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
