#version 100

/// Layout
attribute vec3 position;
attribute vec2 ST;
attribute vec3 normal;

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
