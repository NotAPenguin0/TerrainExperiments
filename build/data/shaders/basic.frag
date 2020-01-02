#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

layout(location = 2) uniform sampler2D height_map;

void main() {
    FragColor = vec4(1, 0, 0, 1);
    float height = 1 - texture(height_map, TexCoords).r;
    if (height > 0.8) {
        FragColor = vec4(0.345098, 0.368627, 0.45098, 1) * (height / 0.8);
    } else if (height > 0.2) {
        FragColor = vec4(0, 0.709804, 0.176471, 1) * (1 - (height - 0.2) / 0.8);
    } else if (height < 0.2) {
        FragColor = vec4(0.0235294, 0.176471, 0.780392, 1) * (height / 0.2);
    }
}