#include <iostream>
#include <string>
#include <fstream>

#include <stb/stb_image.h>

#include "gl_core.hpp"

std::string read_file(const char* path) {
    using namespace std::literals::string_literals;
    std::ifstream f(path);
    if (!f.good()) {
        throw std::runtime_error("Failed to open file: "s + path);
    }
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

unsigned int create_shader_stage(GLenum stage, const char* source) {
    using namespace std::literals::string_literals;
    unsigned int shader = glCreateShader(stage);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infolog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infolog);
        throw std::runtime_error("Failed to compile shader:\n"s + source + "\nReason: "s + infolog);
    }

    return shader;
}

unsigned int create_shader(const char* vertex, const char* fragment) {
    using namespace std::literals::string_literals;
    unsigned int vtx = create_shader_stage(GL_VERTEX_SHADER, vertex);
    unsigned int frag = create_shader_stage(GL_FRAGMENT_SHADER, fragment);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vtx);
    glAttachShader(prog, frag);

    glLinkProgram(prog);
    int success;
    char infolog[512];
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(prog, 512, nullptr, infolog);
        throw std::runtime_error("Failed to link shader.\nReason: "s + infolog);
        return 0;
    }

    glDeleteShader(vtx);
    glDeleteShader(frag);

    return prog;
}

unsigned int create_texture(const char* path) {
    using namespace std::literals::string_literals;
    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &channels, 4);
    if (!data) {
        throw std::runtime_error("Failed to load image at path "s + path);
    }

    unsigned int tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glTextureStorage2D(tex, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_R, GL_REPEAT);

    stbi_image_free(data);

    return tex;
}

int main() try {
    auto window = init(800, 600, "Terrain Experiments");

    std::string vertex = read_file("data/shaders/basic.vert");
    std::string fragment = read_file("data/shaders/basic.frag");

    unsigned int shader = create_shader(vertex.c_str(), fragment.c_str());

    unsigned int vao;
    unsigned int vbo;
    unsigned int tex_buffer;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tex_buffer);

    float vertices[] = {-1, -1, 0, 0, 1, 0, 1, -1, 0};
    glNamedBufferData(vbo, 9 * sizeof(float), vertices, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    float tex_coords[] = {0, 0, 0.5, 1, 1, 0};
    glNamedBufferData(tex_buffer, 6 * sizeof(float), tex_coords, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayVertexBuffer(vao, 1, tex_buffer, 0, 2 * sizeof(float));
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 1, 1);

    // prepare for rendering
    glBindVertexArray(vao);
    glUseProgram(shader);

    unsigned int tex = create_texture("data/textures/pengu.png");

    glBindTextureUnit(0, tex);

    while(!glfwWindowShouldClose(window)) {
        try {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwPollEvents();
            glfwSwapBuffers(window);
        } catch(std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }
    }

} catch(std::exception const& e) {
    std::cerr << e.what() << std::endl;
}