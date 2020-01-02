#ifndef TERRAINS_CAMERA_HPP_
#define TERRAINS_CAMERA_HPP_

#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 pos);

    void update(float dt);

    glm::mat4 view_matrix();

    float mouse_sensitivity;
    float move_speed;

    glm::vec3 position;
    glm::vec3 rotation = glm::vec3(0.0, 270.0, 0.0);

private:
    glm::vec3 front = glm::vec3(0, 0, -1);
    glm::vec3 up = glm::vec3(0, 1, 0);
};

#endif