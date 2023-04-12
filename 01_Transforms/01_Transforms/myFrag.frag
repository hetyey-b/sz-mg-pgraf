#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

uniform vec3 alternativeColor;
uniform float colorFactor;

void main()
{
	fs_out_col = vec4(mix (vs_out_col, alternativeColor, colorFactor), 1 );
}
