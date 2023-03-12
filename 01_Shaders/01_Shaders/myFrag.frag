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
    vec4 col1 = vec4(0.0,0.0,0.2,1.0);
    vec4 col2 = vec4(1.0,0.0,0.0,1.0);

    return mix(col1, col2, between0and1);
}

float mandelbrot(vec2 inp) {
    float tmp_real = 0.0;
    float tmp_imag = 0.0;
    int i = 0;
    const int MAX_ITERATION = 10000;

    while(tmp_real * tmp_real + tmp_imag * tmp_imag < 4.0 && i < MAX_ITERATION) {
        float new_real = tmp_real * tmp_real - tmp_imag * tmp_imag + inp.x;
        float new_imag = 2.0 * tmp_real * tmp_imag + inp.y;
        tmp_real = new_real;
        tmp_imag = new_imag;
        i++;
    }

    if (length(vec2(tmp_real, tmp_imag)) > 2.0) {
        return 0.0;
    } else {
        return 1.0;
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

    // vec2 iranyV = vec2(cos(radians(angle_degree)), sin(radians(angle_degree)));

    // 4. feladat
    // fs_out_col = vec4(myFunc(dot(iranyV, vs_out_normCoords)));

    // 5. feladat
    // fs_out_col = vec4(colorize(myFunc(dot(iranyV, vs_out_normCoords))));

    // Hazi feladat
    fs_out_col = vec4(colorize(mandelbrot(vs_out_normCoords)));
}


// 1. feladat: rajzoljuk ki fehérrel a téglalapot!

// 2. feladat: rajzoljuk ki az origó középpontú, 1 sugarú kört! Mit kell tenni, ha nem a
//    körlapot, hanem csak a körvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 3. feladat: uniform változók - animált kör sugár!

// 4. feladat: komplex számok....
