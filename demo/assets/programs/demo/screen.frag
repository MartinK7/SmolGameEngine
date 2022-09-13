#version 460 core
out vec4 FragColor;
uniform sampler2D textureScreen;

in vec2 passST;

void main()
{
	FragColor = texture(textureScreen, passST);
}
