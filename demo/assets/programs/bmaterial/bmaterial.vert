#version 460 core

/// Layout
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;

/// Outputs
out vec2 ioST;
out vec3 ioNormal;
out vec3 ioFragmentPosition;
out vec3 ioCameraPosition;

/// Uniforms
uniform mat4 matrixModel;
uniform mat4 matrixCamera;
uniform vec3 cameraPosition;

/// Functions
void main()
{
    vec4 fragmentPosition = matrixModel * vec4(position, 1.0);

    ioFragmentPosition = fragmentPosition.xyz;
    gl_Position = matrixCamera * fragmentPosition;

    ioST = ST;
    ioNormal = mat3(transpose(inverse(matrixModel))) * normal;
    ioCameraPosition = cameraPosition;
}
