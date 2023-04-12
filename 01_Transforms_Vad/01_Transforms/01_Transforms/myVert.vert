#version 130

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_col;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 world;
uniform mat4 view; // uniform: Egész renderelés során állandóak mindenkire
uniform mat4 proj;
uniform mat4 color; // lekérdezzük initbe, és ezt használjuk színnek valahogy

void main()
{
	gl_Position = proj * view * world * vec4( vs_in_pos, 1 );
	vs_out_col = vs_in_col;
}
