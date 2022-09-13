#version 460 core
out vec4 FragColor;
uniform sampler2D textureAlbedo;

sample smooth in vec2 passST;
in vec3 passNormal;
in vec4 fragmentPosition;
in vec3 passLightPosition;
in vec3 passCameraPosition;

void main()
{
	FragColor = vec4(1.0);
}
