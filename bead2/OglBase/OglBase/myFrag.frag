#version 140

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_normal;
in vec2 vs_out_tex;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

// textúra mintavételező objektum
uniform sampler2D texImage;

uniform vec3 camera_pos;

// fenyforras tulajdonsagok
uniform vec3 lightDir;
float spotAngle = 2 * 3.14152f / 24.0f; // ekkora szögtartományban világít a spotlámpánk

uniform vec4 La = vec4(0.15f, 0.15f, 0.15f, 1);
uniform vec4 Ld = vec4(0.7f, 0.7f, 0.7f, 1);
uniform vec4 Ls = vec4(1, 1, 1, 1);

uniform float specular_power = 16;

uniform float lightConstantAttenuation    = 1.0;
uniform float lightLinearAttenuation      = 0.0;
uniform float lightQuadraticAttenuation   = 0.0;
uniform float light_battery = 100.0;

uniform bool this_is_cone = false; // éppen a kúpot rajzoljuk-e ki?
uniform bool light_switch = true; // be van-e kapcsolva a spotlámpa?

// anyag tulajdonsagok

uniform vec4 Ka = vec4(1, 1, 1, 1);
uniform vec4 Kd = vec4(1, 1, 1, 1);
uniform vec4 Ks = vec4(1, 1, 1, 1);

uniform float Shininess = 0.1;

uniform vec3 eye_pos = vec3(0, 15, 15);

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
    vec4 diffuse = vec4(0, 0, 0, 1);
    vec4 specular = vec4(0, 0, 0, 1);

    vec4 ambient = La * Ka;

    float spoti;

    vec3 toLight = normalize(camera_pos - vs_out_pos);
    float angle = acos(dot(-lightDir, normalize(toLight)));

    spoti = angle < spotAngle ? 1 : 0;
    vec3 normal = normalize(vs_out_normal);
    float di = clamp(dot(toLight, normal), 0.0f, 1.0f);
    diffuse = Ld * Kd * di;

    if (di > 0)
    {
        vec3 e = normalize(eye_pos - vs_out_pos);
        vec3 r = reflect(-toLight, normal);
        float si = pow(clamp(dot(e, r), 0.0f, 1.0f), specular_power);
        specular = Ls * Ks * si;
    }

    // a kúpot csak textúrázzuk
    if (this_is_cone)
    {
        fs_out_col = texture(texImage, vs_out_tex.st);
    }
    else if (light_switch && light_battery > 0.0)
    {
        // ha van fényünk, akkor a diffúz és spekuláris fény erõssége spoti-tól függ
        fs_out_col = (ambient + spoti * (diffuse + specular)) * texture(texImage, vs_out_tex.st);
    }
    else
    {
        // egyébként nincs diffúz és spekuláris komponens, csak ambiens
        fs_out_col = ambient * texture(texImage, vs_out_tex.st);
    }
}
