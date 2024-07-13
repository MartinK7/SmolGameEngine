#version 100
precision mediump float;

/// Declarations
struct sge_pointLight_s
{
    vec3 position;
};

/// Uniforms
uniform sge_pointLight_s sge_pointLight;

/// Input
varying vec3 ioFragmentPosition;

highp vec3 floatToColor(highp float f)
{
    f *= 16777216.0; // 256.0 * 256.0 * 256.0
    highp vec3 color;
    color.b = floor(f / 65536.0); // 256.0 * 256.0
    color.g = floor(mod(f, 65536.0) / 256.0);
    color.r = mod(f, 256.0);
    return color / 255.0;
}

void main()
{
    float far_plane = 100.0;

    // get distance between fragment and light source
    float lightDistance = length(ioFragmentPosition - sge_pointLight.position);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragColor = vec4(floatToColor(lightDistance), 1.0);
}
