#version 100
precision mediump float;

/// Declarations
struct sge_bmaterial_s
{
    vec3 colorAlbedo;
    sampler2D textureAlbedo;
    sampler2D textureNormal;
    samplerCube cubemapEnvironment;
    float mixColorTextureAlbedo;
    float glossy;
};

struct sge_pointLight_s
{
    vec3 position;
    vec3 color;
    float intensity;
    samplerCube cubemapDepthmap;
};

struct ambientLight_s
{
    vec3 color;
    float intensity;
};

/// Fragment output

/// Uniforms
uniform sge_bmaterial_s sge_material;
uniform sge_pointLight_s sge_pointLight;
uniform samplerCube cubemapBackground;

/// Input
varying vec2 ioST;
varying vec3 ioNormal;
varying vec3 ioFragmentPosition;
varying vec3 ioCameraPosition;

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

/// Functions
vec3 tonemap(vec3 x)
{
    #if 1
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

float colorToFloat(highp vec3 color)
{
    highp vec3 shiftedColor = color * 255.0;
    return dot(shiftedColor, vec3(1.0, 256.0, 65536.0)) / 16777216.0;
}

float shadowCalculate(samplerCube cubemapDepthmap, vec3 fragmentToLight)
{
    float currentDepth = length(fragmentToLight);
    float bias = 0.1;
    float closestDepth = colorToFloat(textureCube(sge_pointLight.cubemapDepthmap, fragmentToLight).rgb) * 100.0;
    return step(closestDepth, currentDepth - bias);
}

float shadow()
{
    vec3 fragmentToLight = ioFragmentPosition - sge_pointLight.position;

    float shadow = 0.0;
    float offset = 0.01;

    shadow += shadowCalculate(sge_pointLight.cubemapDepthmap, fragmentToLight + vec3(0.0, offset, 0.0));
    shadow += shadowCalculate(sge_pointLight.cubemapDepthmap, fragmentToLight + vec3(0.0, -offset, -offset));
    shadow += shadowCalculate(sge_pointLight.cubemapDepthmap, fragmentToLight + vec3(offset, -offset, offset));
    shadow += shadowCalculate(sge_pointLight.cubemapDepthmap, fragmentToLight + vec3(-offset, -offset, offset));
    shadow /= 4.0;

    return clamp(shadow, 0.0, 1.0);
}

/// Mavarying function
void main()
{
    ambientLight_s ambientLight;
    ambientLight.intensity = 0.2;
    ambientLight.color = vec3(1.0);

    vec3 normal = normalize(ioNormal);

    vec3 colorGlossy = textureCube(sge_material.cubemapEnvironment, reflect(ioFragmentPosition - ioCameraPosition, normal)).rgb;

    vec3 sge_pointLightDirection = normalize(sge_pointLight.position - ioFragmentPosition);
    vec3 colorDiffuse = (mix(sge_material.colorAlbedo, texture2D(sge_material.textureAlbedo, ioST).rgb, sge_material.mixColorTextureAlbedo)) * sge_pointLight.color * ((1.0 - shadow()) * sge_pointLight.intensity + ambientLight.intensity) * max(dot(normal, sge_pointLightDirection), 0.0);

    vec3 colorAmbient = ambientLight.color * ambientLight.intensity;

    vec3 color = mix(colorDiffuse, colorGlossy, sge_material.glossy);

    color = tonemap(color);

    gl_FragColor = vec4(color, 1.0);
}
