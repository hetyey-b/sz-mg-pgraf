#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
}

// asztal geometria letrehozasa
void CMyApp::InitTableGeometry()
{
	static const float SIZE = 16.0f;

	Vertex vert[] =
	{
		 
		//              x,     y,       z,            nx, ny,  nz,           s, t
		{ glm::vec3(-SIZE, -2.0,   SIZE),glm::vec3(0.0,1.0,0.0), glm::vec2(0.0,0.0) },
		{ glm::vec3( SIZE, -2.0,   SIZE),glm::vec3(0.0,1.0,0.0), glm::vec2(1.0,0.0) },
		{ glm::vec3(-SIZE, -2.0,  -SIZE),glm::vec3(0.0,1.0,0.0), glm::vec2(0.0,1.0) },
		{ glm::vec3( SIZE, -2.0,  -SIZE),glm::vec3(0.0,1.0,0.0), glm::vec2(1.0,1.0) }
	};

	// indexpuffer adatai
	GLushort indices[] =
	{
		// 1. háromszög
		0,1,2,
		// 2. háromszög
		2,1,3,
	};

	m_table_index_count = sizeof(indices) / sizeof(GLushort);

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_table_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_table_vaoID);

	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_table_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_table_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		sizeof(vert),		// ennyi bájt nagyságban
		vert,	// erről a rendszermemóriabeli címről olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat

	// index puffer létrehozása
	glGenBuffers(1, &m_table_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_table_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	);

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2); // ez lesz majd a szín
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3)));

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void CMyApp::InitShaders()
{
	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés előtt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonosítója, amiből egy változóhoz szeretnénk hozzárendelést csinálni
							0,				// a VAO-beli azonosító index
							"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation( m_programID, 1, "vs_in_norm");
	glBindAttribLocation( m_programID, 2, "vs_in_tex");

	// illesszük össze a shadereket (kimenő-bemenő változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, VertexShaderErrorMessage.data());
		std::cerr << "[glLinkProgram] Shader linking error:\n" << &VertexShaderErrorMessage[0] << std::endl;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );
}

void CMyApp::InitTextures()
{
	m_tableTextureID = TextureFromFile("../Normals_Vad/Assets/Seamless_Carved_Wood_Blocks_Texturise.jpg");
	// mintavételezés beállításai
	glBindTexture(GL_TEXTURE_2D, m_tableTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilineáris szürés nagyításkor (ez az alapértelmezett)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilineáris szűrés a mipmap-ekböl kicsinyítéskor
	// mi legyen az eredmény, ha a textúrán kívülröl próbálunk mintát venni?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // vízszintesen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // függölegesen
	glBindTexture(GL_TEXTURE_2D, 0);

	m_sphereTextureID = TextureFromFile("../Normals_Vad/Assets/grey-marble.png");
	// mintavételezés beállításai
	glBindTexture(GL_TEXTURE_2D, m_sphereTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilineáris szürés nagyításkor (ez az alapértelmezett)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilineáris szűrés a mipmap-ekböl kicsinyítéskor
	// mi legyen az eredmény, ha a textúrán kívülröl próbálunk mintát venni?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // vízszintesen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // függölegesen
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	//
	// geometria letrehozasa
	//
	InitTableGeometry();
	InitSphereGeometry();

	// shaderek
	InitShaders();

	// texturak
	InitTextures();
	//
	// egyéb inicializálás
	//

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_world   = glGetUniformLocation( m_programID, "world");
	m_loc_worldIT = glGetUniformLocation(m_programID, "worldIT");
	m_loc_view    = glGetUniformLocation( m_programID, "view" );
	m_loc_proj    = glGetUniformLocation( m_programID, "proj" );

	// textura lekerdezese
	m_loc_tex = glGetUniformLocation(m_programID, "texImage");

	// fenyeles

	m_loc_camera_pos = glGetUniformLocation(m_programID, "camera_pos");

	m_loc_light_pos = glGetUniformLocation(m_programID, "light_pos");
	m_loc_La = glGetUniformLocation(m_programID, "La" ); 
	m_loc_Ld = glGetUniformLocation(m_programID, "Ld" ); 
	m_loc_Ls = glGetUniformLocation(m_programID, "Ls" ); 

	m_loc_lightConstantAttenuation =  glGetUniformLocation(m_programID, "lightConstantAttenuation" );
	m_loc_lightLinearAttenuation = 	  glGetUniformLocation(m_programID, "lightLinearAttenuation" );
	m_loc_lightQuadraticAttenuation = glGetUniformLocation(m_programID, "lightQuadraticAttenuation" );

	m_loc_Shininess = glGetUniformLocation(m_programID, "Shininess");

	// kamera
	m_camera.SetView(
		glm::vec3(0.0, 0.0, 5.0),	// honnan nézzük a színteret	   - eye
		glm::vec3(0.0, 0.0, 0.0),   // a színtér melyik pontját nézzük - at
		glm::vec3(0.0, 1.0, 0.0));  // felfelé mutató irány a világban - up

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_table_vboID);
	glDeleteBuffers(1, &m_table_iboID);
	glDeleteVertexArrays(1, &m_table_vaoID);

	glDeleteBuffers(1, &m_sphere_vboID);
	glDeleteBuffers(1, &m_sphere_iboID);
	glDeleteVertexArrays(1, &m_sphere_vaoID);

	glDeleteTextures(1, &m_tableTextureID);
	glDeleteTextures(1, &m_sphereTextureID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	
	static Uint32 last_time = SDL_GetTicks();

	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

    float timeElapsedInSec = last_time / 1000.0f;

	// nézeti transzformáció lekérdezése
	m_matView = m_camera.GetViewMatrix();

    if (m_scene_isLampOn) {
	    glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
        // Feny tulajdonsagok
        m_lightPos = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        m_lightAmbient = glm::vec3(0.2f);
        m_lightDiffuse = glm::vec3(1.0f);
        m_lightSpecular = glm::vec3(1.0f);

        m_lightConstantAttenuation = 1.0;
        m_lightLinearAttenuation = 0.0;
        m_lightQuadraticAttenuation = 0.0;
    } else {
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glm::vec3 fireflyPos = glm::vec3(2.2f,2.0f*sin(timeElapsedInSec) + 1.0f,0.0f);

        m_lightPos = glm::vec4(fireflyPos, 1.0f);
        m_lightAmbient = glm::vec3(0.1f);
        m_lightDiffuse = glm::vec3(0.1f, 0.99f, 0.61f);
        m_lightSpecular = glm::vec3(0.1f, 0.99f, 0.61f);

        m_lightConstantAttenuation = 1.0;
        m_lightLinearAttenuation = 0.0;
        m_lightQuadraticAttenuation = 0.5;
    }
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa, ebben a projektben a teljes programot jelöli, hisz nem váltunk a shaderek között
	glUseProgram( m_programID );

	// shader parameterek beállítása
	/*

	GLM transzformációs mátrixokra példák:
		glm::rotate<float>( szög, glm::vec3(tengely_x, tengely_y, tengely_z) ) <- tengely_{xyz} körüli elforgatás
		glm::translate<float>( glm::vec3(eltol_x, eltol_y, eltol_z) ) <- eltolás
		glm::scale<float>( glm::vec3(s_x, s_y, s_z) ) <- skálázás

	*/

	glm::mat4 matWorldIT = glm::transpose(glm::inverse(m_matWorld));
	
	glUniformMatrix4fv( m_loc_world,// erre a helyre töltsünk át adatot
						1,			// egy darab mátrixot
						GL_FALSE,	// NEM transzponálva
						&(m_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	glUniformMatrix4fv( m_loc_worldIT, 1, GL_FALSE, &( matWorldIT[0][0]));
	glUniformMatrix4fv( m_loc_view,    1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,    1, GL_FALSE, &( m_matProj[0][0]) );

	// beállítjuk, hogy a fragment shaderben a "texImage" nevű mintavételező a 0-ás számú legyen, ...
	glUniform1i(m_loc_tex,  0);

	glUniform3fv(m_loc_camera_pos, 1, &m_camera.GetEye()[0]);

	glUniform4fv(m_loc_light_pos, 1, &m_lightPos[0]);

	glUniform3fv(m_loc_La, 1, &m_lightAmbient[0]);
	glUniform3fv(m_loc_Ld, 1, &m_lightDiffuse[0]);
	glUniform3fv(m_loc_Ls, 1, &m_lightSpecular[0]);

	glUniform1f(m_loc_lightConstantAttenuation, m_lightConstantAttenuation);
	glUniform1f(m_loc_lightLinearAttenuation, m_lightLinearAttenuation);
	glUniform1f(m_loc_lightQuadraticAttenuation, m_lightQuadraticAttenuation);

	// asztal rajzolasa

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_table_vaoID);

	// textúra beállítása
	// aktiváljuk a 0-ás textúramintavételezőt (innenőt a gl*Texture* hívások erre vonatkoznak)
	// ez akkor fontos, ha több textúrát akarunk egyszerre használni, ha egyszerre csak egy kell,
	// akkor ezt elhagyva automatikusan a 0-ás mintavételező aktív
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tableTextureID);

	// csillogas beallitasa

	glUniform1f(m_loc_Shininess, 4.0);

	// kirajzolás
	//A draw hívásokhoz a VAO és a program bindolva kell legyenek (glUseProgram() és glBindVertexArray())

	glDrawElements(GL_TRIANGLES, m_table_index_count, GL_UNSIGNED_SHORT, 0);
	
	// textúrak kikapcsolása
	// Jelenleg ay 1-es unit aktiv,
	glBindTexture(GL_TEXTURE_2D, 0);
	// igy vissza kell valtani 0-ra

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// gomb rajzolasa

	glBindVertexArray(m_sphere_vaoID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sphereTextureID);
	
	// csillogas
	glUniform1f(m_loc_Shininess, m_shininess);

	glDrawElements(GL_TRIANGLES, m_sphere_index_count, GL_UNSIGNED_SHORT, 0);

	// textúrak kikapcsolása
	glBindTexture(GL_TEXTURE_2D, 0);


	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE) {
        m_scene_isLampOn = !m_scene_isLampOn;
    } 
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
    m_shininess = glm::clamp(m_shininess+wheel.y, 1, 20);
}

// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
	m_matProj = m_camera.GetProj();
}
