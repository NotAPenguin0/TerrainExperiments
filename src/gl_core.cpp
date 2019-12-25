#include "gl_core.hpp"

#include <iostream>

static void glfw_error_callback(int errc, const char* desc) {
    std::cerr << "[GLFW] Error: Code " << errc << ": " << desc << std::endl;
}

static void gl_error_callback([[maybe_unused]] GLenum source, 
                       [[maybe_unused]] GLenum type, 
                       [[maybe_unused]] GLuint id,
                       [[maybe_unused]] GLenum severity,
                       [[maybe_unused]] GLsizei length, 
                       [[maybe_unused]] GLchar const* message,
                       [[maybe_unused]] void const* user_data) {
    
    std::string err_type = "Unknown error";
    if (type == GL_DEBUG_TYPE_ERROR) {
        err_type = "Error";
    } else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) {
        err_type = "Deprecated behavior";
    } else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) {
        err_type = "Undefined behavior";
    } else if (type == GL_DEBUG_TYPE_PORTABILITY) {
        err_type = "Non portable functionality";
    } else if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        err_type = "Performance issue";
    } else if (type == GL_DEBUG_TYPE_MARKER) {
        err_type = "Command stream annotation";
    } else if (type == GL_DEBUG_TYPE_OTHER) {
        err_type = "Other error";
    }

    std::string severity_string = "Unknown";
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) severity_string == "Info";
    else if (severity == GL_DEBUG_SEVERITY_LOW) severity_string = "Warning";
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM) severity_string = "Error";
    else if (severity == GL_DEBUG_SEVERITY_HIGH) severity_string = "Critical error";

    std::cerr << "[OpenGL] " << severity_string << ": " << message << std::endl;
}


GLFWwindow* init(size_t w, size_t h, const char* title) {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (!window) {
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return nullptr;
    }

    return window;
}

void create_debug_output() {
        // enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_error_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}