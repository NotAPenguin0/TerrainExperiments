#version 460 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    float val = texture(tex, TexCoords).r;
    if (val < 0.45) {
        // water
        FragColor = vec4(0.027451, 0.501961, 0.921569, 1.0);
    } else if (val < 0.50) {
        // beach
        FragColor = vec4(0.929412, 0.980392, 0.470588, 1.0);
    } else if (val < 1.0) {
        // regular ground
        FragColor = vec4(0.00784314, 0.870588, 0.0941176, 1.0);
    } else if (val < 0.82) {
        // mountain
        FragColor = vec4(0.313725, 0.368627, 0.321569, 1.0);
    } else {
        // snow
        FragColor = vec4(1, 1, 1, 1);
    }

    FragColor = vec4(val, val, val, 1.0);
}