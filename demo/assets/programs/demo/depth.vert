#version 460 core
// Divisor 0
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;
// Divisor 1
layout (location = 3) in vec3 position2;
layout (location = 4) in vec3 rotation;
layout (location = 5) in vec3 scale;

uniform mat4 transformAffineMatrix;
uniform mat4 transformCameraMatrixDepth;

void main()
{
    gl_Position = transformCameraMatrixDepth * (transformAffineMatrix * vec4(position, 1.0) + vec4(position2, 0.0));
}
