#version 450

layout(location = 0) out vec4 o_color;

in vec2 i_texCoords;
in vec4 i_color;

uniform float u_time = 0.0;

uniform sampler2D Buffer;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(500, 500);

    vec4 pixel = texture(Buffer, i_texCoords);

    float alpha = pixel.a * i_color.a;

    vec3 color = 0.5 + 0.5 * cos(u_time + uv.xyx + vec3(0,2,4));

    vec4 finalColor;

    if(i_color.r * i_color.g * i_color.b * i_color.a == 1.0)
        finalColor = vec4(color, 1.0) * alpha;
    else
        finalColor = i_color * pixel;
        
    o_color = finalColor;
}
