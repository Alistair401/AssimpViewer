#version 430

in layout(location=0) vec3 local_pos;
in layout(location=1) vec3 normal;
in layout(location=2) vec3 vertex_color;
in layout(location=3) vec2 tex_coord;
in layout(location=4) ivec4 bone_ids;
in layout(location=5) vec4 bone_weights;

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