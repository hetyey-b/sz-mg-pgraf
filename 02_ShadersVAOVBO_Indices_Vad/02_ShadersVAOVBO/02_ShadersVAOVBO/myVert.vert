#version 130

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_col;

void main()
{
	gl_Position = vec4( vs_in_pos, 1 ); // A gl_Position MINDIG vec4 kell hogy legyen. Mivel a bemenő pozíció itt vec3, ezért egy 1.0-val kiegészítjük.
	vs_out_pos = vs_in_pos;
	vs_out_col = vs_in_col;
}