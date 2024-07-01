#version 100
precision mediump float;

/// Fragment output
//out vec4 FragColor;

/// Uniforms
uniform samplerCube cubemapBackground;

/// Input
varying vec3 ioFragmentPosition;

/// Functions
void main()
{
    gl_FragColor = vec4(textureCube(cubemapBackground, ioFragmentPosition).rgb, 1.0);
}
