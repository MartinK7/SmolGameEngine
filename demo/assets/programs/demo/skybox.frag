#version 460 core
out vec4 FragColor;

in vec3 passPosition;

uniform samplerCube cubemapSkybox;

void main()
{
    FragColor = texture(cubemapSkybox, passPosition);
}
