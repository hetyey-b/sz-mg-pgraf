#version 330 core

// pipeline-ból bejövő per-fragment attribútumok
// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

// textúra mintavételező objektum
uniform sampler2D texImage;

uniform vec3 camera_pos;

// fenyforras tulajdonsagok
uniform vec4 light_pos = vec4( 0.0, 0.0, 1.0, 0.0);

uniform vec3 La = vec3(0.0, 0.0, 0.0 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );

// anyag tulajdonsagok

uniform vec3 Ka = vec3( 1.0 );
uniform vec3 Kd = vec3( 1.0 );

uniform float Shininess = 0.1;

uniform vec3 Remmisive = vec3(0.0);

void main()
{
	vec3 normal = normalize(vs_out_norm);

	vec3 Ambient = La * Ka;

	vec3 ToLight	= normalize(light_pos.xyz);
	
	float DiffuseFactor = max(dot(ToLight,normal), 0.0);
	vec3 Diffuse = DiffuseFactor * Ld * Kd;
	
	fs_out_col = vec4(Ambient + Diffuse, 1) * texture(texImage, vs_out_tex)
        + vec4(Remmisive, 0.0);
}
