#version 130

// bemeneti változó - most a vertex shader-ből (vagyis ottani out)
in vec4 vs_out_col; 
in vec2 vs_out_normCoords;

// kimeneti változó - a fragment színe
out vec4 fs_out_col;

const float PI = 3.14159265359;
const float angle_degree = 30.0;

float myFunc(float t) {
    return (0.5 * cos(2*PI*t)) + 0.5;
}

vec4 colorize(float between0and1) {
    vec4 col1 = vec4(1.0,0.0,0.0,1.0);
    vec4 col2 = vec4(0.0,0.0,1.0,1.0);

    return mix(col1, col2, between0and1);
}

float manderbrot(vec2 inp) {
    const int ITERATIONS = 1000;
    int iteration = 0;

    float x = inp.x;
    float y = inp.y;

    while ((inp.x*inp.x) + (inp.y*inp.y) <= 2*2 && iteration < ITERATIONS) {
        x = (inp.x*inp.x) - (inp.y*inp.y) + inp.x;
        y = 2*inp.x*inp.y + inp.y;
        iteration++;
    }

    if (length(vec2(x,y)) < 2) {
        return 1.0;
    } else {
        return 0.0;
    }
}

void main()
{
	//				  R, G, B, A
	// fs_out_col = vs_out_col;

	//				  R, G, B, A
	// fs_out_col = vec4(vec3(1.0) - vs_out_col.rgb, vs_out_col.a);

    // 2. feladat
    // fs_out_col = vec4(vs_out_normCoords, 0.0, 1.0);

    // 3. feladat
    // fs_out_col = vec4(myFunc(vs_out_normCoords.x));

    vec2 iranyV = vec2(cos(radians(angle_degree)), sin(radians(angle_degree)));

    // 4. feladat
    // fs_out_col = vec4(myFunc(dot(iranyV, vs_out_normCoords)));

    // 5. feladat
    // fs_out_col = vec4(colorize(myFunc(dot(iranyV, vs_out_normCoords))));

    // Hazi feladat
    fs_out_col = vec4(colorize(manderbrot(vs_out_normCoords)));
}


// 1. feladat: rajzoljuk ki fehérrel a téglalapot!

// 2. feladat: rajzoljuk ki az origó középpontú, 1 sugarú kört! Mit kell tenni, ha nem a
//    körlapot, hanem csak a körvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 3. feladat: uniform változók - animált kör sugár!

// 4. feladat: komplex számok....
