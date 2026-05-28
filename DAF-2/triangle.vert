#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 FragPos;

void main() {
    // Убираем transform, используем только model
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPos;

    // Позиция фрагмента в мировом пространстве
    FragPos = worldPos.xyz;

    // Корректное преобразование нормалей
    Normal = normalMatrix * aNormal;
}