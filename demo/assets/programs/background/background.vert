#version 100

/// Laout
attribute vec3 position;
attribute in vec2 ST;
attribute in vec3 normal;

/// Outputs
varying vec3 ioFragmentPosition;

/// Uniforms
uniform mat4 matrixCamera;

/// Functions
void main()
{
    vec4 fragmentPosition = vec4(position, 1.0);

    ioFragmentPosition = fragmentPosition.xyz;
    gl_Position = matrixCamera * fragmentPosition;
}
