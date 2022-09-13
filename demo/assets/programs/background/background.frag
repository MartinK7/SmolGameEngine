#version 460 core

/// Fragment output
out vec4 FragColor;

/// Uniforms
uniform samplerCube cubemapBackground;

/// Input
in vec3 ioFragmentPosition;

/// Functions
void main()
{
    FragColor = vec4(texture(cubemapBackground, ioFragmentPosition).rgb, 1.0);
}
