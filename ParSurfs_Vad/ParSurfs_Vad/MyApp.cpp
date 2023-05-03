#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
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
	m_TextureID = TextureFromFile("../ParSurfs_Vad/Assets/color_checkerboard.png");
	// mintavételezés beállításai
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
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
	InitParamSurfGeometry();

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
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_iboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteTextures(1, &m_TextureID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	
	static Uint32 last_time = SDL_GetTicks();

	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	const float ElapsedTimeInSec = static_cast<float>(last_time) / 1000.0f;

	// nézeti transzformáció lekérdezése
	m_matView = m_camera.GetViewMatrix();

	m_lightPos = glm::vec4(m_camera.GetEye(), 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, 
		(polygonModeWireframe ? GL_LINE : GL_FILL ) );
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

	
	// gomb rajzolasa

	glBindVertexArray(m_vaoID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	
	// csillogas
	glUniform1f(m_loc_Shininess, 64.0f );

	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_SHORT, 0);

	// textúrak kikapcsolása
	glBindTexture(GL_TEXTURE_2D, 0);


	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_F1)
	{
		polygonModeWireframe = !polygonModeWireframe;
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
}

// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
	m_matProj = m_camera.GetProj();
}
