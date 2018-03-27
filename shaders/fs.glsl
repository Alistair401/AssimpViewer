#version 430

in vec3 _color;
in vec2 _tex_coord;

// uniform sampler2D tex_data;

out vec4 frag_color;

void main()
{
    // frag_color = texture(tex_data,_tex_coord) * vec4(_color,1.0);
    frag_color = vec4(_color,1.0);
}