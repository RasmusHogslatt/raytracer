
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
add_subdirectory(glfw)

set(BUILD_FRAMEWORK            OFF CACHE INTERNAL "Build package as Mac OS X Framework bundle.")
set(SYSTEM_IRRXML              OFF CACHE INTERNAL "Use system installed Irrlicht/IrrXML library.")
set(BUILD_DOCS                 OFF CACHE INTERNAL "Build documentation using Doxygen.")
set(INJECT_DEBUG_POSTFIX       ON  CACHE INTERNAL "Inject debug postfix in .a/.so lib names")
set(IGNORE_GIT_HASH            ON  CACHE INTERNAL "Don't call git to get the hash.")

option(GLM_QUIET "No CMake Message" ON)
option(GLM_TEST_ENABLE "Build unit tests" OFF)
# set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "GLM shared lib build")
# set(BUILD_STATIC_LIBS ON  CACHE INTERNAL "GLM static build")
add_subdirectory(glm)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # Needed when using clang since GLM fails to detect c++11/14 support
    # https://github.com/g-truc/glm/issues/620
    target_compile_definitions(glm INTERFACE GLM_FORCE_CXX14)
endif()

add_subdirectory(stb)
add_subdirectory(glad)
add_subdirectory(imgui)

