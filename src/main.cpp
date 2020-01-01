#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>


#include <stb/stb_image.h>

#include "gl_core.hpp"
#include "noise.hpp"
#include "math.hpp"

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

// std::vector<unsigned char> get_combined_perlin_noise_buffer(size_t w, size_t h, size_t iterations) {
//     static float perlin_2d_min = -std::sqrt(0.5f);
//     static float perlin_2d_max = std::sqrt(0.5f);

//     std::vector<unsigned char> buffer(w * h, 0);
//     for (int it = 0; it < iterations; ++it) {
//         size_t const detail = std::pow(2, it);
//         float it_weight = (1.0f / std::pow(2, it + 1));
//         gradient_grid gradients(detail, detail);
//         for (int y = 0; y < h; ++y) {
//             for (int x = 0; x < w; ++x) {
//                 int i = y * w + x;
//                 float value = perlin_noise((float)x / w * detail, (float)y / h * detail, gradients);
                
//                 value = map_value(value, perlin_2d_min, perlin_2d_max, 0.0f, 1.0f);
//                 value *= 255;

//                 buffer[i] += value * it_weight;
//             }
//         }
//     }
//     return buffer;
// }

unsigned int perlin_noise_texture(size_t w, size_t h, size_t scale) {
    PerlinNoise noise(scale);
    std::vector<unsigned char> noise_buf = noise.get_buffer(w, h);
//    std::vector<unsigned char> noise = get_combined_perlin_noise_buffer(w, h, 3);
    unsigned int tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureStorage2D(tex, 1, GL_R8, w, h);
    glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, noise_buf.data());
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return tex;
}

int main() try {
    auto window = init(800, 800, "Terrain Experiments");

    std::string vertex = read_file("data/shaders/basic.vert");
    std::string fragment = read_file("data/shaders/basic.frag");

    unsigned int shader = create_shader(vertex.c_str(), fragment.c_str());

    unsigned int vao;
    unsigned int vbo;
    unsigned int tex_buffer;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tex_buffer);

    float vertices[] = {-1, 1, 0, -1, -1, 0, 1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};
    glNamedBufferData(vbo,  18 * sizeof(float), vertices, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    float tex_coords[] = {0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1};
    glNamedBufferData(tex_buffer, 12 * sizeof(float), tex_coords, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayVertexBuffer(vao, 1, tex_buffer, 0, 2 * sizeof(float));
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 1, 1);

    // prepare for rendering
    glBindVertexArray(vao);
    glUseProgram(shader);

    unsigned int tex = create_texture("data/textures/pengu.png");
    size_t detail = 1;
    unsigned int noise = perlin_noise_texture(800, 800, detail);
    glBindTextureUnit(0, noise);

    bool held = false;

    while(!glfwWindowShouldClose(window)) {
        try {
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                if (!held) {
                    held = true;
                    ++detail;
                    glDeleteTextures(1, &noise);
                    noise = perlin_noise_texture(800, 800, detail);
                    glBindTextureUnit(0, noise);
                }
            } else {
                held = false;
            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && detail > 1) {
                if (!held) {
                    held = true;
                    --detail;
                    glDeleteTextures(1, &noise);
                    noise = perlin_noise_texture(800, 800, detail);
                    glBindTextureUnit(0, noise);
                }
            } else {
                held = false;
            }

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwPollEvents();
            glfwSwapBuffers(window);
        } catch(std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }
    }

} catch(std::exception const& e) {
    std::cerr << e.what() << std::endl;
}