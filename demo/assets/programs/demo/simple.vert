#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 offset;

out vec2 passST;

void main()
{
    gl_Position = vec4(position + offset, 1.0);
    passST = ST;
}
