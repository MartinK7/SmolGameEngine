#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;

out vec2 passST;
out vec3 passNormal;

uniform mat4 transformAffineMatrix;
uniform mat4 transformCameraMatrix;

void main()
{
    gl_Position = transformCameraMatrix * transformAffineMatrix * vec4(position, 1.0);
    passST = ST;
    passNormal = normal;
}
