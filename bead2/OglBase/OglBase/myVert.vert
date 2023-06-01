#version 130

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_normal;
out vec2 vs_out_tex;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * world * vec4( vs_in_pos, 1 );
	vs_out_pos  = (world   * vec4(vs_in_pos,  1)).xyz;
	vs_out_normal = (worldIT * vec4(vs_in_norm, 0)).xyz;

	vs_out_tex = vs_in_tex;
}