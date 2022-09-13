#version 460 core
out vec4 FragColor;
uniform sampler2D textureAlbedo;

sample smooth in vec2 passST;
in vec3 passNormal;
in vec4 fragmentPosition;
in vec3 passLightPosition;
in vec3 passCameraPosition;

vec3 color()
{
	return vec3(texture(textureAlbedo, passST).x);
}

void main()
{
	vec3 normal = normalize(passNormal);

	// Sun light
	vec3 sunDirection = vec3(0.0, -1.0, -0.0);
	//
	sunDirection = -sunDirection;
	float sunIntensity = max(dot(normal, sunDirection), 0.0);

	// Ambient light
	float ambientIntensity = 0.5;

	// Diffuse light
	vec3 lightDir = normalize(passLightPosition - fragmentPosition.xyz);
	float diffuseIntensity = max(dot(normal, lightDir), 0.0);

	// Specular light
	float specularStrength = 0.5;
	//
	vec3 viewDir = normalize(passCameraPosition - fragmentPosition.xyz);
	vec3 reflectDir = reflect(-lightDir, normal);
	//
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float specularIntensity = specularStrength * spec;

	vec3 colorOut = vec3(0.0);
	//colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * sunIntensity;
	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * ambientIntensity;
	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * diffuseIntensity;
	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * specularIntensity;
	colorOut = color();
	colorOut  = pow(colorOut, vec3(2.2));

	float saturateDetected = min((step(1.0, colorOut.r) + step(1.0, colorOut.g) + step(1.0, colorOut.b)), 1);
	FragColor = (saturateDetected) * vec4(1.0, 0.0, 0.0, 1.0) + (1.0 - saturateDetected) * vec4(colorOut, 1);
}
