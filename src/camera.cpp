#include "camera.hpp"

#include "input.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera(glm::vec3 pos) : position(pos) {}

void Camera::update(float dt) {

    if (RawInput::get_mouse_button(MouseButton::Right).down) {
        float xoffset = RawInput::get_mouse().xoffset;
        float yoffset = RawInput::get_mouse().yoffset;
        xoffset *= mouse_sensitivity;
        yoffset *= mouse_sensitivity;

        rotation.x += yoffset;
        rotation.y += xoffset;
        static float max_pitch = 89.0f;
        static float min_pitch = -89.0f;
        if (rotation.x > max_pitch) rotation.x = max_pitch;
        if (rotation.x < min_pitch) rotation.x = min_pitch;
    }

    // Process rotation
    auto cos_pitch = std::cos(glm::radians(rotation.x));
    auto cos_yaw = std::cos(glm::radians(rotation.y));
    auto sin_pitch = std::sin(glm::radians(rotation.x));
    auto sin_yaw = std::sin(glm::radians(rotation.y));
    front.x = cos_pitch * cos_yaw;
    front.y = sin_pitch;
    front.z = cos_pitch * sin_yaw;
    front = glm::normalize(front);

    if (RawInput::get_mouse_button(MouseButton::Left).down) {
        float speed = move_speed * dt;
        float horizontal = RawInput::get_mouse().xoffset;
        float vertical = RawInput::get_mouse().yoffset;

        glm::vec3 cam_right = glm::normalize(glm::cross(up, front));
        position += (speed * front * vertical);
        position += (speed * cam_right * horizontal);
    }
}


glm::mat4 Camera::view_matrix() {
    static glm::vec3 world_up = glm::vec3(0, 1, 0);
    glm::vec3 right = glm::normalize(glm::cross(up, front));
    up = glm::cross(front, right);
    return glm::lookAt(position, position + front, up);
}