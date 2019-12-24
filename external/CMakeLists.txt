add_subdirectory("glfw")

target_compile_options(glfw PUBLIC -w)

project(glad)

set(GLAD_GL_VERSION 460)

add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/glad${GLAD_GL_VERSION}/src/glad.c")
target_compile_options(glad PUBLIC -w)
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad${GLAD_GL_VERSION}/include")

set(TERRAINS_LINK_LIBRARIES
    ${TERRAINS_LINK_LIBRARIES}
    glfw
    glad
    PARENT_SCOPE
)

set(TERRAINS_INCLUDE_DIRECTORIES
    ${TERRAINS_INCLUDE_DIRECTORIES}
    "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/glad${GLAD_GL_VERSION}/include"
)