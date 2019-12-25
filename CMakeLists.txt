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

target_compile_options(Terrains PUBLIC -std=c++17 -Wall)
target_include_directories(Terrains PUBLIC ${TERRAINS_INCLUDE_DIRECTORIES})
target_link_libraries(Terrains PUBLIC ${TERRAINS_LINK_LIBRARIES})
