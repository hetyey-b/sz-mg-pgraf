#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_ibID  = 0;
	m_vaoWireframeID = 0;
	m_iboWireframeID = 0;
	m_programID = 0;

	isWireframe = false;
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)


    const int n = 8;
    float angle = 2 * M_PI / n;
    Vertex vert[n+1];

    
    // kozepso vertex
    vert[0] = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };

    // kulso vertexek
    for (int i = 1; i <= n; i++) {
        float x = cosf(i * angle);
        float y = sinf(i * angle);
        vert[i] = { glm::vec3(x, y, 0.0f), glm::vec3((float)i/n, (float)(n-i)/n, 0.0f) };
    }

    GLushort indices[n*3];

    // triangle fan
    for (int i = 0; i < n; i++) {
        indices[i*3]   = 0;
        indices[i*3+1] = i+1;
        indices[i*3+2] = i+2;
    }

    // last triangle
    indices[(n-1)*3+2] = 1;

	//
	// geometria létrehozása
	//
	
	/* static const float SQRT_3_PER_2 = sqrtf( 3.0f ) / 2.0f; */
	/*  */
	/* Vertex vert[] = */
	/* { */
	/* 	glm::vec3(  0.0,           0.0, 0.0 ), glm::vec3( 1, 1, 1 ), */
	/* 	glm::vec3(  1.0,           0.0, 0.0 ), glm::vec3( 1, 0, 0 ), */
	/* 	glm::vec3(  0.5,  SQRT_3_PER_2, 0.0 ), glm::vec3( 1, 1, 0 ), */
	/* 	glm::vec3( -0.5,  SQRT_3_PER_2, 0.0 ), glm::vec3( 0, 1, 0 ), */
	/* 	glm::vec3( -1.0,           0.0, 0.0 ), glm::vec3( 0, 1, 1 ), */
	/* 	glm::vec3( -0.5, -SQRT_3_PER_2, 0.0 ), glm::vec3( 0, 0, 1 ), */
	/* 	glm::vec3(  0.5, -SQRT_3_PER_2, 0.0 ), glm::vec3( 1, 0, 1 ), */
	/* }; */
	/*  */
	/* GLushort indices[] = */
	/* { */
	/* 	0,1,2, */
	/* 	0,2,3, */
	/* 	0,3,4, */
	/* 	0,4,5, */
	/* 	0,5,6, */
	/* 	0,6,1 */
	/* }; */

	// 1 db VAO foglalása
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beállítasa aktívnak
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
	glGenBuffers( 1, &m_ibID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );


	// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szám
		GL_FLOAT,		// adatok típusa
		GL_FALSE,		// normalizált legyen-e
		sizeof(Vertex),	// stride (0=egymás után)
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		(GLuint)1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); // feltöltöttük a VBO-t is, ezt is vegyük le

	// Wireframe
	/*  */
	/* glLineWidth(3.0); // Vonal rajzolásnál a vastagságot állítja. */
	/*  */
	/* GLushort indicesWireframe[] = */
	/* { */
	/* 	0,1,1,2,2,0, // 1.hsz */
	/* 	0,2,2,3,3,0, // 2.hsz */
	/* 	0,3,3,4,4,0, // 3.hsz */
	/* 	0,4,4,5,5,0, // 4.hsz */
	/* 	0,5,5,6,6,0, // 5.hsz */
	/* 	0,6,6,1,1,0, // 6.hsz */
	/* }; */
	/*  */
	/* // 1 db VAO foglalása */
	/* glGenVertexArrays(1, &m_vaoWireframeID); */
	/* // a frissen generált VAO beállítasa aktívnak */
	/* glBindVertexArray(m_vaoWireframeID); */
	/*  */
	/* // !!FIGYELEM: Ne generlájunk új VBO-t, hisz ugyan azokat a vertexeket tartalmazná. */
	/* // Viszont szükséges, hogy az új wireframe VAO is "tudjon róla". */
	/* // Az Attrib Pointerek beállításánál fontos, hogy egy VBO Bind-olva legyen! */
	/* glBindBuffer(GL_ARRAY_BUFFER, m_vboID);  */
	/*  */
	/* // Viszont az új index buffer új adatokat tartalmaz. */
	/* glGenBuffers(1, &m_iboWireframeID); */
	/* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboWireframeID); */
	/* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesWireframe), indicesWireframe, GL_STATIC_DRAW); */
	/*  */
	/* // Az attribútumokat ehhez a VAOhoz is be kell állítani */
	/* glEnableVertexAttribArray(0); // ez lesz majd a pozíció */
	/* glVertexAttribPointer( */
	/* 	(GLuint)0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be */
	/* 	3,				// komponens szám */
	/* 	GL_FLOAT,		// adatok típusa */
	/* 	GL_FALSE,		// normalizált legyen-e */
	/* 	sizeof(Vertex),	// stride (0=egymás után) */
	/* 	0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül */
	/* ); */
	/*  */
	/* // a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín) */
	/* glEnableVertexAttribArray(1); // ez lesz majd a szín */
	/* glVertexAttribPointer( */
	/* 	(GLuint)1, */
	/* 	3, */
	/* 	GL_FLOAT, */
	/* 	GL_FALSE, */
	/* 	sizeof(Vertex), */
	/* 	(void*)(sizeof(glm::vec3))); */
	/*  */
	/* glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le */
	/* glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le */
	/* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le */

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

	// attributomok osszerendelese a VAO es shader kozt
	glBindAttribLocation( m_programID, 0, "vs_in_pos");
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenő-bemenő változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkelés ellenőrzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, VertexShaderErrorMessage.data());

		std::cerr << "[glLinkProgram] Shader linking error:\n" << &VertexShaderErrorMessage[0] << std::endl;
	}

	// már nincs ezekre szükség
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteBuffers( 1, &m_ibID );
	glDeleteBuffers(1, &m_vaoWireframeID);
	glDeleteBuffers(1, &m_iboWireframeID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolása, ebben a projektben a teljes programot jelöli, hisz nem váltunk a shaderek között
	glUseProgram( m_programID );

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)

	// kirajzolás
	//A draw hívásokhoz a VAO és a program bindolva kell legyenek (glUseProgram() és glBindVertexArray())
	
	if (isWireframe)
	{
		glBindVertexArray(m_vaoWireframeID);
		glDrawElements(GL_LINES, // Vonalakat rajzolunk
			36,
			GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glBindVertexArray( m_vaoID );
		glDrawElements( GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0 );
	}

	// VAO kikapcsolása
	glBindVertexArray(0);

	// shader kikapcsolása
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE) isWireframe = !isWireframe;
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

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
	glViewport(0, 0, _w, _h );
}
