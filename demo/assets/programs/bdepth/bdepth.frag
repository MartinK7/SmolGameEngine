#version 460 core

/// Declarations
struct sge_pointLight_s
{
    vec3 position;
};

/// Uniforms
uniform sge_pointLight_s sge_pointLight;

/// Input
in vec3 ioFragmentPosition;

/// Main function
void main()
{
    float far_plane = 100.0;

    // get distance between fragment and light source
    float lightDistance = length(ioFragmentPosition - sge_pointLight.position);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
