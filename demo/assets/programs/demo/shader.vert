#version 460 core
// Divisor 0
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 ST;
layout (location = 2) in vec3 normal;

out vec2 passST;
out vec3 passNormal;
out vec3 passLightPosition;
out vec3 passCameraPosition;

out vec4 fragmentPosition;
out vec4 fragmentPositionDepthMap;

uniform mat4 transformAffineMatrix;
uniform mat3 transformNormalMatrix;
uniform mat4 transformCameraMatrix;
uniform mat4 transformCameraMatrixDepth;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
    fragmentPosition = transformAffineMatrix * vec4(position, 1.0);
    fragmentPositionDepthMap = transformCameraMatrixDepth * fragmentPosition;
    gl_Position = transformCameraMatrix * fragmentPosition;

    passST = ST;
    passNormal = transformNormalMatrix * normal;
    passLightPosition = lightPosition;
    passCameraPosition = cameraPosition;
}
