#version 130

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_normal;
in vec2 vs_out_tex0;

// kimen� �rt�k - a fragment sz�ne
out vec4 fs_out_col;

uniform bool this_is_cone = false; // �ppen a k�pot rajzoljuk-e ki?
uniform bool light_switch = true; // be van-e kapcsolva a spotl�mpa?
uniform bool is_sharp = true; // "�les" legyen-e a spotl�mpa f�ny�nek a hat�ra

//
// uniform v�ltoz�k
//

// sz�nt�r tulajdons�gok
uniform vec3 eye_pos = vec3(0, 15, 15);
uniform vec3 light_pos = vec3( 0, 5, 0 );
uniform vec3 lightDir = vec3(0, -1, 0);
float spotAngle = 2 * 3.14152f / 16.0f; // ekkora sz�gtartom�nyban vil�g�t a spotl�mp�nk
uniform vec4 La = vec4(0.15f, 0.15f, 0.15f, 1);

// f�nytulajdons�gok
uniform vec4 Ld = vec4(0.7f, 0.7f, 0.7f, 1);
uniform vec4 Ls = vec4(1, 1, 1, 1);

// anyagtulajdons�gok
uniform vec4 Ka = vec4(1, 1, 1, 1);
uniform vec4 Kd = vec4(1, 1, 1, 1);
uniform vec4 Ks = vec4(1, 1, 1, 1);
uniform float specular_power = 16;
uniform sampler2D texImage;

void main()
{

	// inicializ�lunk
	vec4 diffuse = vec4(0, 0, 0, 1);
	vec4 specular = vec4(0, 0, 0, 1);

	//
	// ambiens sz�n sz�m�t�sa
	//
	vec4 ambient = La * Ka;

	// *************************
	// spot l�mpa intenzit�sa
	// *************************

	// Ha egy fregmenst nem eg�szen olyan sz�gben �ri a f�nysug�r (-toLight), mint amerre n�z a spotl�mpa (lightDir),
	// akkor a spotl�mpa k�r�n k�v�l eshet: a r� sz�molt f�ny intenzit�s�t cs�kkentj�k
	// ezt az intenzit�st sz�moljuk ki spoti-ban
	float spoti;
	
	// toLight a fregmensb�l a f�nyforr�sba mutat� vektor (-toLight pedig a f�nyforr�sb�l a fregmensbe)
	// a spotl�mp�nk lightDir ir�nyba n�z
	vec3 toLight = normalize(light_pos - vs_out_pos);
	// angle fejezi ki a k�t vektor sz�g�t radi�nban
	float angle = acos(dot(-toLight, lightDir));

	// ha angle t�l nagy, akkor spoti 1-n�l kisebb �rt�k� -> gyeng�bb a f�ny �ri a fregmenst!

	if(is_sharp){
		// "�les" f�nyk�r: vagy van f�ny, vagy nincs
		spoti = angle < spotAngle ? 1 : 0;
	} else {
		// https://en.wikipedia.org/wiki/Sigmoid_function
		// finomabb �tmenetet kapunk egy megfelel�en megv�lasztott szigmoid f�gv�nnyel:
		// min�l nagyobb a sz�g, ann�l kisebb �rt�k� spoti, de nem hirtelen ugrik
		float ex = exp(-64 * (angle - spotAngle));
		spoti  = ex / (ex + 1);
	}

	//
	// diff�z sz�n sz�m�t�sa
	//

	/* seg�ts�g:
		- normaliz�l�s: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
		- skal�ris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
		- clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/
	vec3 normal = normalize( vs_out_normal );
	float di = clamp( dot( toLight, normal), 0.0f, 1.0f );
	diffuse = Ld*Kd*di;

	//
	// f�nyfoltk�pz� sz�n
	//

	/* seg�ts�g:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/

	if ( di > 0 )
	{
		vec3 e = normalize( eye_pos - vs_out_pos );
		vec3 r = reflect( -toLight, normal );
		float si = pow( clamp( dot(e, r), 0.0f, 1.0f ), specular_power );
		specular = Ls*Ks*si;
	}

	// a k�pot csak text�r�zzuk
	if(this_is_cone){
		fs_out_col = texture(texImage, vs_out_tex0.st);
	} else if (light_switch){
		// ha van f�ny�nk, akkor a diff�z �s spekul�ris f�ny er�ss�ge spoti-t�l f�gg
		fs_out_col = (ambient + spoti*(diffuse + specular)) * texture(texImage, vs_out_tex0.st);
	} else {
		// egy�bk�nt nincs diff�z �s spekul�ris komponens, csak ambiens
		fs_out_col = ambient * texture(texImage, vs_out_tex0.st);
	}

}