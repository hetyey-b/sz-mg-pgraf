#version 140

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
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );

uniform float lightConstantAttenuation    = 1.0;
uniform float lightLinearAttenuation      = 0.0;
uniform float lightQuadraticAttenuation   = 0.0;

// anyag tulajdonsagok

uniform vec3 Ka = vec3( 1.0 );
uniform vec3 Kd = vec3( 1.0 );
uniform vec3 Ks = vec3( 1.0 );

uniform float Shininess = 0.1;

/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
				reflect(beérkező_vektor, normálvektor);
		- pow: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
				pow(alap, kitevő);
*/

void main()
{
	vec3 normal = normalize( vs_out_norm );

	vec3 ToLight;
	float LightDistance=0.0;
	if ( light_pos.w == 0 )
	{
		ToLight	= normalize(light_pos.xyz);
	}
	else 
	{
		ToLight	= light_pos.xyz - vs_out_pos;
		LightDistance = length(ToLight);
		ToLight = normalize(ToLight);
	}

	float Attenuation = 1.0 / ( lightConstantAttenuation + lightLinearAttenuation * LightDistance + lightQuadraticAttenuation * LightDistance * LightDistance);

	vec3 Ambient = La * Ka;

	float DiffuseFactor = max(dot(ToLight,normal), 0.0) * Attenuation;
	vec3 Diffuse = DiffuseFactor * Ld * Kd;

	vec3 viewDir = normalize( camera_pos - vs_out_pos );
	vec3 reflectDir = reflect( -ToLight, normal );

	float SpecularFactor = pow(max( dot( viewDir, reflectDir) ,0.0), Shininess) * Attenuation;
	vec3 Specular = SpecularFactor*Ls*Ks;

	fs_out_col = vec4( Ambient+Diffuse+Specular, 1.0 ) * texture(texImage, vs_out_tex);
	//fs_out_col = vec4( normal* 0.5 +0.5,1.0);
}
