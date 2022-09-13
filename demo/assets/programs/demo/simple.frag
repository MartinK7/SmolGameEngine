#version 460 core
out vec4 FragColor;
uniform sampler2D textureAlbedo1;
uniform sampler2D textureAlbedo2;

in vec2 passST;

void main()
{
	FragColor = ((sign(passST.x - 0.5) + 1.0) / 2.0) * vec4(texture(textureAlbedo1, passST).xyz, 0) + (1.0 - (sign(passST.x - 0.5) + 1.0) / 2.0) * vec4(texture(textureAlbedo2, passST).xyz, 0);
}
