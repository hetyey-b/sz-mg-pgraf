#version 330 core

// VBO-ból érkező változók
//in vec3 vs_in_pos;
//in vec3 vs_in_norm;
//in vec2 vs_in_tex;

in vec2 vs_in_uv;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külső paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

vec3 GetPos(float u, float v)
{
	vec3 pos = vec3(-10.0, 0.0, 10.0) + vec3( 20.0, 0.0, -20.0) * vec3(u, 0.0, v);

	return pos;
}

GetNorm(float u, float v)
{
	vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
	vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

	return normalize(cross(du, dv));
}

void main()
{
    vec3 vs_in_pos = GetPos(vs_in_uv.x, vs_in_uv.y);
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	
	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(GetNorm(vs_in_uv.x, vs_in_uv.y), 0)).xyz;
	vs_out_tex = vs_in_uv;
}
