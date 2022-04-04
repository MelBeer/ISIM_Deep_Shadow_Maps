
Include(FetchContent)

# Put all your sources here EXCEPT YOUR MAIN.CC
set(SRC
        "src/Renderer.cpp"
        "src/geometry.cpp"
        "src/Image.cpp"
        "src/printer.cpp"
        "src/Camera.cpp"
    "src/"
)

# SETUP
cmake_minimum_required(VERSION 3.12)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3") # -fsanitize=address")

# LIBRARIES
# ADD_SUBDIRECTORY(assimp)

# 
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.2.3
)
FetchContent_MakeAvailable(assimp)

# RAYTRACER
add_executable(raytracer)
target_sources(raytracer PRIVATE "src/main.cc" ${SRC})
target_link_libraries(raytracer PRIVATE ${LIBRARIES})
TARGET_LINK_LIBRARIES(raytracer assimp::assimp)
target_include_directories(raytracer PRIVATE "${assimp_SOURCE_DIR}/include" )


# STATIC TARGET "You don't need give a single cr*p about it until you need" -friend of mine
# if (CMAKE_BUILD_TYPE STREQUAL "Release")
#     add_executable(raytracer-static)
#     target_sources(raytracer-static PRIVATE "src/main.cc" ${SRC})
#     target_link_libraries(raytracer-static PRIVATE ${LIBRARIES} -static)
#  endif()
