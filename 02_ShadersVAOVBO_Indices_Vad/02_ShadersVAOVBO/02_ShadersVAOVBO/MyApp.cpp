#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <iostream>

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

int n = 18;
bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)


    std::vector<Vertex> vert;
    
    vert.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) });
    for (int i = 1; i < n+1; i++) {
        float angle = ((float)(i-1) * 2 * M_PI) / (float)n;
        float x = cosf(angle);
        float y = sinf(angle);
        vert.push_back({ glm::vec3(x, y, 0.0f), glm::vec3((float)i/n, (float)(n-i)/n, 0.0f) });
    }
    
    std::vector<GLushort> indices;

    /* GLushort indices[n*3]; */

    for (int i = 0; i < n; i++) {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
        /* indices[i*3]   = 0; */
        /* indices[i*3+1] = i+1; */
        /* indices[i*3+2] = i+2; */
    }

    indices[(n-1)*3+2] = 1;

	// 1 db VAO foglalása
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beállítasa aktívnak
	glBindVertexArray(m_vaoID);

	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert[0])*vert.size(),		// ennyi bájt nagyságban
				  vert.data(),	// erről a rendszermemóriabeli címről olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat
	
	// index puffer létrehozása
	glGenBuffers( 1, &m_ibID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices[0] ) * indices.size(), indices.data(), GL_STATIC_DRAW );


	// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként 
    // lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szám
		GL_FLOAT,		// adatok típusa
		GL_FALSE,		// normalizált legyen-e
		sizeof(Vertex),	// stride (0=egymás után)
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után 
    // sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
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

	// wireframe
	
	
    std::vector<GLushort> indicesWireframe;

    for (int i = 1; i <= n; i++) {
        indicesWireframe.push_back(0);
        indicesWireframe.push_back(i);
        indicesWireframe.push_back(i);
        indicesWireframe.push_back((i % n) + 1);
        indicesWireframe.push_back((i % n) + 1);
        indicesWireframe.push_back(0);
    }    
	
	glGenVertexArrays(1, &m_vaoWireframeID);
	glBindVertexArray(m_vaoWireframeID);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); 
	
	glGenBuffers(1, &m_iboWireframeID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboWireframeID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesWireframe[0]) * indicesWireframe.size(), indicesWireframe.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		(GLuint)0,				
		3,		
		GL_FLOAT,
		GL_FALSE,	
		sizeof(Vertex),
		0				
	);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		(GLuint)1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));
	
	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

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
			n*6,
			GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glBindVertexArray( m_vaoID );
		glDrawElements( GL_TRIANGLES, n*3, GL_UNSIGNED_SHORT, 0 );
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
