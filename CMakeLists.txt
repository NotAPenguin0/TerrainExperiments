cmake_minimum_required(VERSION 3.12)

project(Terrains)

set(TERRAINS_SOURCES "")
set(TERRAINS_LINK_LIBRARIES "")
set(TERRAINS_INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

add_subdirectory("src")
add_subdirectory("external")

add_executable(Terrains ${TERRAINS_SOURCES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Terrains PUBLIC -std=c++17 -Wall)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Terrains PUBLIC /std:c++17 /W0)
endif()

target_include_directories(Terrains PUBLIC ${TERRAINS_INCLUDE_DIRECTORIES})
target_link_libraries(Terrains PUBLIC ${TERRAINS_LINK_LIBRARIES})
