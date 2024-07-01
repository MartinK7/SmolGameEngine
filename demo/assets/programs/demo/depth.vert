#version 100
// Divisor 0
attribute vec3 position;
attribute vec2 ST;
attribute vec3 normal;
// Divisor 1
attribute vec3 position2;
attribute vec3 rotation;
attribute vec3 scale;

uniform mat4 transformAffineMatrix;
uniform mat4 transformCameraMatrixDepth;

/// Uniforms
uniform mat4 matrixModel;
uniform mat4 matrixCamera;

void main()
{
    vec4 fragmentPosition = matrixModel * vec4(position, 1.0);
    gl_Position = matrixCamera * fragmentPosition;
}
