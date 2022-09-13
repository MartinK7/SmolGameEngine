#version 460 core

/// Layout
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;

/// Outputs

/// Uniforms
uniform mat4 matrixModel;
uniform mat4 matrixCamera;

/// Functions
void main()
{
    vec4 fragmentPosition = matrixModel * vec4(position, 1.0);

    gl_Position = matrixCamera * fragmentPosition;
}
