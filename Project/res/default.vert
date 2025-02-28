#version 110

varying vec2 i_texCoords;
varying vec4 i_color;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    i_texCoords = (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;

    i_color = gl_Color;
}
