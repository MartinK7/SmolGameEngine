#version 460 core
// Divisor 0
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;

out vec3 passPosition;

uniform mat4 transformCameraMatrix;
uniform vec3 cameraPosition;

void main()
{
    passPosition = position;
    gl_Position = transformCameraMatrix * vec4(position + cameraPosition, 1.0);
}
