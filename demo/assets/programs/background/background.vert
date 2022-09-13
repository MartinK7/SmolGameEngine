#version 460 core

/// Laout
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;

/// Outputs
out vec3 ioFragmentPosition;

/// Uniforms
uniform mat4 matrixCamera;

/// Functions
void main()
{
    vec4 fragmentPosition = vec4(position, 1.0);

    ioFragmentPosition = fragmentPosition.xyz;
    gl_Position = matrixCamera * fragmentPosition;
}
