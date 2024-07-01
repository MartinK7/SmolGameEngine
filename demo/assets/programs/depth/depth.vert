#version 100

/// Layout
attribute in vec3 position;
attribute in vec2 ST;
attribute in vec3 normal;

/// Outputs
varying vec3 ioFragmentPosition;

/// Uniforms
uniform mat4 matrixModel;
uniform mat4 matrixCamera;

/// Main function
void main()
{
    vec4 fragmentPosition = matrixModel * vec4(position, 1.0);

    ioFragmentPosition = fragmentPosition.xyz;

    gl_Position = matrixCamera * fragmentPosition;
}
