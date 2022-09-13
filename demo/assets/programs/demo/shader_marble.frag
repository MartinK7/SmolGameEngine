#version 460 core
out vec4 FragColor;
uniform sampler2D textureAlbedo;

sample smooth in vec2 passST;
in vec3 passNormal;
in vec4 fragmentPosition;
in vec3 passLightPosition;
in vec3 passCameraPosition;

/*
void main()
{
	//FragColor = texture(textureAlbedo, passST);
	FragColor = vec4(passST, 0.0, 1.0);
}
*/



// -----------------------------------------------------------------------------
// THIS CODE IS FROM:
// https://www.shadertoy.com/view/XsscW8
vec3 mod289_1(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0;}vec2 mod289_1(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec3 permute_1(vec3 x) { return mod289_1(((x * 34.0) + 1.0) * x);}
float Bias(float val, float bias) { return (val / ((((1. / bias) - 2.) * (1. - val)) + 1.));}vec3 Bias(vec3 val, float bias) { return (val / ((((1. / bias) - 2.) * (1. - val)) + 1.));}
float snoise(vec2 v) { const vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439); vec2 i = floor(v + dot(v, C.yy)); vec2 x0 = v - i + dot(i, C.xx); vec2 i1; i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0); vec4 x12 = x0.xyxy + C.xxzz; x12.xy -= i1; i = mod289_1(i); vec3 p = permute_1(permute_1(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0)); vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0); m = m * m; m = m * m; vec3 x = 2.0 * fract(p * C.www) - 1.0; vec3 h = abs(x) - 0.5; vec3 ox = floor(x + 0.5); vec3 a0 = x - ox; m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h); vec3 g; g.x = a0.x * x0.x + h.x * x0.y; g.yz = a0.yz * x12.xz + h.yz * x12.yw; return 130.0 * dot(m, g);}
vec3 cosPalette(float t, vec3 a, vec3 b, vec3 c, vec3 d) { return a + b * cos(6.28318 * (c * t + d));}
float multifractalA(vec2 point, float H, float lacunarity, float frequency, float octaves, float offset) { float value = 1.0; float rmd = 0.0; float pwHL = pow(lacunarity, -H); float pwr = 1.0; for(int i = 0; i < 65535; i++) {  value *= pwr * snoise(point * frequency) + offset;  pwr *= pwHL;  point *= lacunarity;  if(i == int(octaves) - 1)   break;   } rmd = octaves - floor(octaves); if(rmd != 0.0)  value += (rmd * snoise(point * frequency) * pwr); return value;}
float SinXY(vec2 pos) { return (sin(pos.x) + sin(pos.y) + 2.) / 4.;}
vec2 Warp(vec2 pos, float wx, float wy, float x, float y) { return pos + vec2(wx * x, wy * y);}
vec2 Translate(vec2 pos, vec2 shift) { return pos + shift;}
vec2 Scale(vec2 pos, vec2 scale) { return pos * scale;}
float Gain(float val, float gain) { if(val < 0.5) {  return Bias(val * 2., gain) / 2.; } else {  return Bias(val * 2. - 1., 1. - gain) / 2. + 0.5; }}vec3 Gain(vec3 val, float gain) { return vec3(Gain(val.x, gain), Gain(val.y, gain), Gain(val.z, gain));}

vec3 color(in vec2 pos)
{
	return cosPalette(Gain(SinXY(Warp(Translate(Scale(pos, vec2(66., 0.57)), vec2(1., 1.)), 8., 2.2, multifractalA(Scale(pos, vec2(-2.4, 1.7)), 1.6, 2., 3., 5.28, 0.8), multifractalA(Translate(Scale(pos, vec2(-2.4, 1.7)), vec2(10., -0.4)), 1.8, 2., 2.7, 60.8, 0.8))), 0.19), vec3(0.5, 0.5, 0.5), vec3(0.38, 0.52, 0.59), vec3(0.36, 0.33, 0.27), vec3(0.56, 0.57, 0.69));
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
	//	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * sunIntensity;
	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * ambientIntensity;
	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * diffuseIntensity;
	colorOut += 0.5 * vec3(1.0, 1.0, 1.0) * specularIntensity;
	colorOut *= color(passST * 50.0);
	//colorOut  = pow(colorOut, vec3(2.2));

	float saturateDetected = min((step(1.0, colorOut.r) + step(1.0, colorOut.g) + step(1.0, colorOut.b)), 1);
	FragColor = (saturateDetected) * vec4(1.0, 0.0, 0.0, 1.0) + (1.0 - saturateDetected) * vec4(colorOut, 1);
}
