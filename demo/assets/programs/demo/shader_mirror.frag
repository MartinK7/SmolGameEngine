#version 460 core
out vec4 FragColor;
uniform sampler2D textureDepthmap;
uniform samplerCube cubemapEnviroment;
uniform samplerCube cubemapSkybox;

sample smooth in vec2 passST;
in vec3 passNormal;
in vec4 fragmentPosition;
in vec3 passLightPosition;
in vec3 passCameraPosition;
in vec4 fragmentPositionDepthMap;

vec3 rgb2hsv(vec3 c)
{
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float LinearizeDepth(float depth)
{
	float near_plane = 0.1f;
	float far_plane = 20.0f;
	float z = depth * 2.0 - 1.0; // Back to NDC
	return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

vec3 colorBase()
{
	return vec3(0.2, 0.8, 0.5);
}

vec3 colorReflect()
{
	vec3 normal = normalize(passNormal);

	vec3 I = normalize(fragmentPosition.xyz - passCameraPosition);
	vec3 R = reflect(I, normal);
	vec3 colorReflect = texture(cubemapEnviroment, R).rgb;

	return colorReflect;
}

vec3 tonemap(vec3 x)
{
	#if 0
	// HDR
	const float gamma = 2.2;
	// reinhard tone mapping
	// gamma correction
	x = pow(x / (x + vec3(1.0)), vec3(1.0 / gamma));

	// Saturate colors
	x = rgb2hsv(x);
	x.y *= 2.0;
	x = hsv2rgb(x);
	return x;
	#else
	// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return (x * (a * x + b)) / (x * (c * x + d) + e);
	#endif
}

void main()
{
	vec3 normal = normalize(passNormal);

	// Ambient light
	float ambientIntensity = 0.2;

	// Light total
	float lightTotal = 5.0;

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

	// Calcualte shadows
	vec3 projCoords = fragmentPositionDepthMap.xyz / fragmentPositionDepthMap.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = LinearizeDepth(texture(textureDepthmap, projCoords.xy).r);
	float currentDepth = LinearizeDepth(projCoords.z);
	float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0005);
	//
	float shadow = 0.0;
	//
	if(currentDepth - bias > closestDepth && projCoords.z < 1.0) {
		vec2 texelSize = 1.0 / textureSize(textureDepthmap, 0);
		const int WH = 2;
		for(int x = -WH / 2; x <= WH / 2; ++x)
		{
			for(int y = -WH / 2; y <= WH / 2; ++y)
			{
				float pcfDepth = texture(textureDepthmap, projCoords.xy + vec2(x, y) * texelSize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
			}
		}
		shadow /= (WH + 1.0) * (WH + 1.0);
	}
	//
	if(projCoords.z > 1.0) {
		shadow = 0.0;
	}
	//
	diffuseIntensity *= (1.0 - shadow);  // Point light
	specularIntensity *= (1.0 - shadow); // Point light

	vec3 colorBaseAmbientX = colorBase() * ambientIntensity * vec3(0.9, 0.9, 1.0);
	vec3 colorBaseX = colorBase() * ((diffuseIntensity + specularIntensity) * lightTotal);
	vec3 colorReflectX = colorReflect();
	vec3 colorOut = mix(colorBaseX, colorReflectX, 0.5) + colorBaseAmbientX;
	colorOut = tonemap(colorOut);

/*	if(
		projCoords.x >= 0.0 && projCoords.x <= 1.0 &&
		projCoords.y >= 0.0 && projCoords.y <= 1.0
	) {
		colorOut += vec3(0.1, 0.1, 0.0);
	}
*/
	float saturateDetected = min((step(1.0, colorOut.r) + step(1.0, colorOut.g) + step(1.0, colorOut.b)), 1);
	FragColor = (saturateDetected) * vec4(1.0, 0.0, 0.0, 1.0) + (1.0 - saturateDetected) * vec4(colorOut, 1);

	//FragColor = vec4(fragmentPositionDepthMap.xyz / fragmentPositionDepthMap.w, 1.0);
}
