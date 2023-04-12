#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
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

	Vertex vert[] =
	{

		//             x,    y, z             R, G, B
		/* { glm::vec3(-1.0, -1.0, 0), glm::vec3(1, 0, 0) }, */
		/* { glm::vec3( 1.0, -1.0, 0), glm::vec3(0, 1, 0) }, */
		/* { glm::vec3(-1.0,  1.0, 0), glm::vec3(0, 0, 1) }, */
		/* { glm::vec3( 1.0,  1.0, 0), glm::vec3(1, 1, 1) }, */
		/* { glm::vec3( 0.0,  0.0, 1.0), glm::vec3(0, 1, 0) } */
		{ glm::vec3(-1.0,  0.0, -1.0), glm::vec3(0, 0, 0) },
		{ glm::vec3( 1.0,  0.0, -1.0), glm::vec3(1, 0, 0) },
		{ glm::vec3(-1.0,  0.0,  1.0), glm::vec3(0, 0, 1) },
		{ glm::vec3( 1.0,  0.0,  1.0), glm::vec3(1, 0, 1) },
		{ glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0, 1, 0) }
	};

	// indexpuffer adatai
	GLushort indices[] =
	{
		// 1. háromszög
		0,1,2,
		// 2. háromszög
		2,1,3,
        2,3,4,
        3,1,4,
        0,2,4,
        0,4,1,
	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// erről a rendszermemóriabeli címről olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat
	
	// index puffer létrehozása
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
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
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
	glBindAttribLocation( m_programID, 1, "vs_in_col");

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

	//
	// egyéb inicializálás
	//

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_view  = glGetUniformLocation( m_programID, "view" );
	m_loc_proj  = glGetUniformLocation( m_programID, "proj" );

    m_loc_alternativeColor  = glGetUniformLocation( m_programID, "alternativeColor" );
    m_loc_colorFactor       = glGetUniformLocation( m_programID, "colorFactor" );

	// kamera
	m_camera.SetView(
		glm::vec3(0.0, 0.0, 5.0),	// honnan nézzük a színteret	   - eye
		glm::vec3(0.0, 0.0, 0.0),   // a színtér melyik pontját nézzük - at
		glm::vec3(0.0, 1.0, 0.0));  // felfelé mutató irány a világban - up

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_iboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// nézeti transzformáció lekérdezése
	
	static Uint32 last_time = SDL_GetTicks();

	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	m_matView = m_camera.GetViewMatrix();
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
    float timeElapsedInSec = SDL_GetTicks() / 1000.0f;

	m_matWorld = glm::mat4(1.0f);

    static const float s_radius = 0.2f;
    static const float s_rotation = 0.2f;

    float x = (s_radius * timeElapsedInSec) * cos(2.0f * M_PI * s_rotation * timeElapsedInSec);
    float z = (s_radius * timeElapsedInSec) * sin(2.0f * M_PI * s_rotation * timeElapsedInSec);

    m_matWorld = glm::translate(glm::vec3(x,0.0f,z));
	
	// majd küldjük át a megfelelő mátrixokat!
	// Uniform változó bindolása csak a program bindolása után lehetséges! (glUseProgram() )
	glUniformMatrix4fv( m_loc_world,// erre a helyre töltsünk át adatot
						1,			// egy darab mátrixot
						GL_FALSE,	// NEM transzponálva
						&(m_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

    float integerPart;
    float decimalPart = std::modf(timeElapsedInSec, &integerPart);
    bool backwards = ((int)integerPart % 2) == 1;

    glUniform3f(m_loc_alternativeColor,0.5f,0.0f,0.5f);
    if (backwards) {
        glUniform1f(m_loc_colorFactor, 1-decimalPart);
    } else {
        glUniform1f(m_loc_colorFactor, decimalPart);
    }

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
	//A draw hívásokhoz a VAO és a program bindolva kell legyenek (glUseProgram() és glBindVertexArray())

	glDrawElements(GL_TRIANGLES,	   // rajzoljunk ki háromszöglista primitívet
					18,				   // 6 indexet rajzolunk
					GL_UNSIGNED_SHORT, // unsigned short tipusuak
					0);			       // 0 byte offset

    /* m_matWorld = glm::translate(glm::vec3(x,1.0f,z + 0.5f)); */

	/* glUniformMatrix4fv( m_loc_world,// erre a helyre töltsünk át adatot */
	/* 					1,			// egy darab mátrixot */
	/* 					GL_FALSE,	// NEM transzponálva */
	/* 					&(m_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot */
	/* glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) ); */
	/* glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) ); */
	/*  */
	/* glDrawElements(GL_TRIANGLES,	   // rajzoljunk ki háromszöglista primitívet */
	/* 				18,				   // 6 indexet rajzolunk */
	/* 				GL_UNSIGNED_SHORT, // unsigned short tipusuak */
	/* 				0);			       // 0 byte offset */

    static const glm::vec3 fleet_offsets[] = {
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(3.536f, 3.536f, 0.0f),
        glm::vec3(-3.536f, 3.536f, 0.0f),
        glm::vec3(3.536f, -3.536f, 0.0f),
        glm::vec3(-3.536f, -3.536f, 0.0f),
        glm::vec3(-5.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(0.0f, -5.0f, 0.0f),
    };

    for (glm::vec3 offset : fleet_offsets) {
	    glm::mat4 m_tmp_matWorld = glm::mat4(1);

        glUniform3f(m_loc_alternativeColor,0.0f,0.5f,0.5f);

        m_tmp_matWorld = m_matWorld * glm::translate(glm::vec3(offset[0],offset[1],offset[2]));

        glUniformMatrix4fv( m_loc_world,// erre a helyre töltsünk át adatot
                1,			// egy darab mátrixot
                GL_FALSE,	// NEM transzponálva
                &(m_tmp_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
        glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
        glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

        glDrawElements(GL_TRIANGLES,	   // rajzoljunk ki háromszöglista primitívet
                18,				   // 6 indexet rajzolunk
                GL_UNSIGNED_SHORT, // unsigned short tipusuak
                0);			       // 0 byte offset
    }

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
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
}

// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
	m_matProj = m_camera.GetProj();
}