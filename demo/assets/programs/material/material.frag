#version 460 core

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
out vec4 FragColor;

/// Uniforms
uniform sge_bmaterial_s sge_material;
uniform sge_pointLight_s sge_pointLight;
uniform samplerCube cubemapBackground;

/// Input
in vec2 ioST;
in vec3 ioNormal;
in vec3 ioFragmentPosition;
in vec3 ioCameraPosition;

/// Functions
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

float shadow()
{
    const float far_plane = 100.0;

    vec3 fragmentPositionDepthMap = vec3(1.0);

    vec3 fragmentToLight = ioFragmentPosition - sge_pointLight.position;

    // Calcualte shadows
    float currentDepth = length(fragmentToLight);
    //float bias = max(0.001 * (1.0 - dot(ioNormal, fragmentToLight)), 0.1);
    float bias = 0.05;
    //float bias = 0.0;
    //
    float shadow  = 0.0;
    float samples = 4.0;
    float offset  = 0.05;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closestDepth = texture(sge_pointLight.cubemapDepthmap, fragmentToLight + vec3(x, y, z)).r * far_plane;
                if(currentDepth - bias > closestDepth)
                shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
    return clamp(shadow, 0.0, 1.0);
}

/// Main function
void main()
{
    ambientLight_s ambientLight;
    ambientLight.intensity = 0.2;
    ambientLight.color = vec3(1.0);

    vec3 normal = normalize(ioNormal);

    vec3 colorGlossy = texture(sge_material.cubemapEnvironment, reflect(ioFragmentPosition - ioCameraPosition, normal)).rgb;

    vec3 sge_pointLightDirection = normalize(sge_pointLight.position - ioFragmentPosition);
    vec3 colorDiffuse = (mix(sge_material.colorAlbedo, texture(sge_material.textureAlbedo, ioST).rgb, sge_material.mixColorTextureAlbedo)) * sge_pointLight.color * ((1.0 - shadow()) * sge_pointLight.intensity + ambientLight.intensity) * max(dot(normal, sge_pointLightDirection), 0.0);

    vec3 colorAmbient = ambientLight.color * ambientLight.intensity;

    vec3 color = mix(colorDiffuse, colorGlossy, sge_material.glossy);

    FragColor = vec4(tonemap(color), 1.0);
}
