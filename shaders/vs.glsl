#version 430

in layout(location=0) vec3 local_pos;
in layout(location=1) vec3 normal;
in layout(location=2) vec3 vertex_color;
in layout(location=3) vec2 tex_coord;
in layout(location=4) uint bone_ids[4];
in layout(location=8) float bone_weights[4];

layout (std430) buffer bone_buffer
{ 
    mat4 bones[];
};

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 _color;
out vec2 _tex_coord;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(local_pos,1.0);
    _color = vertex_color;
    _tex_coord = tex_coord;
}