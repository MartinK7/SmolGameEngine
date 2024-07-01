#version 100

/// Layout
attribute in vec3 position;
attribute in vec2 ST;
attribute in vec3 normal;

/// Outputs
varying vec2 ioST;
varying vec3 ioNormal;
varying vec3 ioFragmentPosition;
varying vec3 ioCameraPosition;

/// Uniforms
uniform mat4 matrixModel;
uniform mat4 matrixCamera;
uniform vec3 cameraPosition;

mat4 transpose(mat4 m)
{
    return mat4(
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]
    );
}

// Function to calculate the inverse of a 4x4 matrix
mat4 inverse(mat4 m)
{
    float
        a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
        a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
        a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
        a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3];

    float
        b00 = a00 * a11 - a01 * a10,
        b01 = a00 * a12 - a02 * a10,
        b02 = a00 * a13 - a03 * a10,
        b03 = a01 * a12 - a02 * a11,
        b04 = a01 * a13 - a03 * a11,
        b05 = a02 * a13 - a03 * a12,
        b06 = a20 * a31 - a21 * a30,
        b07 = a20 * a32 - a22 * a30,
        b08 = a20 * a33 - a23 * a30,
        b09 = a21 * a32 - a22 * a31,
        b10 = a21 * a33 - a23 * a31,
        b11 = a22 * a33 - a23 * a32;

    float det =
        b00 * b11
        - b01 * b10
        + b02 * b09
        + b03 * b08
        - b04 * b07
        + b05 * b06;

    return mat4(
        a11 * b11 - a12 * b10 + a13 * b09,
        -a01 * b11 + a02 * b10 - a03 * b09,
        a31 * b05 - a32 * b04 + a33 * b03,
        -a21 * b05 + a22 * b04 - a23 * b03,
        -a10 * b11 + a12 * b08 - a13 * b07,
        a00 * b11 - a02 * b08 + a03 * b07,
        -a30 * b05 + a32 * b02 - a33 * b01,
        a20 * b05 - a22 * b02 + a23 * b01,
        a10 * b10 - a11 * b08 + a13 * b06,
        -a00 * b10 + a01 * b08 - a03 * b06,
        a30 * b04 - a31 * b02 + a33 * b00,
        -a20 * b04 + a21 * b02 - a23 * b00,
        -a10 * b09 + a11 * b07 - a12 * b06,
        a00 * b09 - a01 * b07 + a02 * b06,
        -a30 * b03 + a31 * b01 - a32 * b00,
        a20 * b03 - a21 * b01 + a22 * b00) / det;
}

/// Functions
void main()
{
    vec4 fragmentPosition = matrixModel * vec4(position, 1.0);

    ioFragmentPosition = fragmentPosition.xyz;
    gl_Position = matrixCamera * fragmentPosition;

    ioST = ST;
    ioNormal = mat3(transpose(inverse(matrixModel))) * normal;
    ioCameraPosition = cameraPosition;
}
