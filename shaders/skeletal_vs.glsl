#version 430

in layout(location=0) vec3 local_pos;
in layout(location=1) vec3 normal;
in layout(location=2) vec3 vertex_color;
in layout(location=3) vec2 tex_coord;
in layout(location=4) uvec4 bone_ids;
in layout(location=5) vec4 bone_weights;

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
    vec4 skinned_vertex = vec4(0,0,0,0);
    skinned_vertex += bones[bone_ids.w] * vec4(local_pos,1.0) * bone_weights.w;
    skinned_vertex += bones[bone_ids.x] * vec4(local_pos,1.0) * bone_weights.x;
    skinned_vertex += bones[bone_ids.y] * vec4(local_pos,1.0) * bone_weights.y;
    skinned_vertex += bones[bone_ids.z] * vec4(local_pos,1.0) * bone_weights.z;
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(skinned_vertex.xyz,1.0);
    _color = vertex_color;
    _tex_coord = tex_coord;
}