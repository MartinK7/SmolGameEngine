#version 460 core
out vec4 FragColor;
uniform sampler2D textureAlbedo;

in vec2 passST;
in vec3 passNormal;

void main()
{
	FragColor = texture(textureAlbedo, passST);
}
