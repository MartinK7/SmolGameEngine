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

vec3 unpackColor(float f) {
    vec3 color;
    color.b = floor(f / 256.0 / 256.0);
    color.g = floor((f - color.b * 256.0 * 256.0) / 256.0);
    color.r = floor(f - color.b * 256.0 * 256.0 - color.g * 256.0);
    // now we have a vec3 with the 3 components in range [0..255]. Let's normalize it!
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
    gl_FragColor = vec4(unpackColor(lightDistance * 256.0 * 256.0 * 256.0), 1.0);
}
