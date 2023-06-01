#include "MyApp.h"
#include "GLUtils.hpp"

#include <GL/GLU.h>
#include <math.h>

#include "ObjParser_OGL3.h"

CMyApp::CMyApp(void)
{
	m_textureID = 0;
	m_mesh = 0;
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);		// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST);	// m�lys�gi teszt bekapcsol�sa (takar�s)

	//
	// geometria letrehozasa
	//

	// TALAJ

	m_vb_ground.AddAttribute(0, 3);
	m_vb_ground.AddAttribute(1, 3);
	m_vb_ground.AddAttribute(2, 2);

	m_vb_ground.AddData(0, -10,  0, -10);
	m_vb_ground.AddData(0,  10,  0, -10);
	m_vb_ground.AddData(0, -10,  0,  10);
	m_vb_ground.AddData(0,  10,  0,  10);

	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);

	m_vb_ground.AddData(2, 0, 0);
	m_vb_ground.AddData(2, 1, 0);
	m_vb_ground.AddData(2, 0, 1);
	m_vb_ground.AddData(2, 1, 1);

	m_vb_ground.AddIndex(1, 0, 2);
	m_vb_ground.AddIndex(1, 2, 3);

	m_vb_ground.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// K�PPAL�ST
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// a k�pot �gy hozzuk l�tre, hogy a cs�csa az orig�ban van
	// �gy majd amikor forgatjuk, a transzform�ci�k r�v�n a cs�cs�n�l fogva fog forogni

	m_vb_cone.AddAttribute(0, 3);
	m_vb_cone.AddAttribute(1, 3);
	m_vb_cone.AddAttribute(2, 2);

	// nulladik vertex: az orig�ban
	m_vb_cone.AddData(0, 0, 0, 0);
	m_vb_cone.AddData(1, 0, 1, 0);
	m_vb_cone.AddData(2, 0.5, 0.5);

	// els�, m�sodik, ..., N-edik vertex
	// az N+1-edik vertexet ugyanoda rakjuk, ahova az els�t
	for (int i = 0; i <= N; ++i){
		float a = i * 2 * 3.14153f / N;
		// poz�ci�: XZ sugar� k�rvonal ment�n, Y = -0.25 magass�gban
		m_vb_cone.AddData(0, 0.25 * cosf(a), -0.25, 0.25 * sinf(a));
		// nem ez a korrekt norm�lvektor!
		// de mivel shader szinten �gysem foglalkozunk a k�p megvil�g�t�s�val, �gy mindegy is
		m_vb_cone.AddData(1, cosf(a), 0, sinf(a));
		// text�r�z�s: a text�ra k�zep�b�l kiv�gunk egy k�rlapot
		m_vb_cone.AddData(2, 0.5*cosf(a) + 0.5, 0.5*sinf(a) + 0.5);
	}

	// indices
	for (int i = 0; i < N; ++i){
		m_vb_cone.AddIndex(0, i + 2, i + 1);
	}

	m_vb_cone.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// K�P ALJA
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	m_vb_cone_bottom.AddAttribute(0, 3);
	m_vb_cone_bottom.AddAttribute(1, 3);
	m_vb_cone_bottom.AddAttribute(2, 2);

	// nulladik vertex: az orig�ban
	m_vb_cone_bottom.AddData(0, 0, -0.25, 0);
	m_vb_cone_bottom.AddData(1, 0, 1, 0);
	m_vb_cone_bottom.AddData(2, 0.5, 0.5);

	// els�, m�sodik, ..., N-edik vertex
	// az N+1-edik vertexet ugyanoda rakjuk, ahova az els�t
	for (int i = 0; i <= N; ++i){
		float a = i * 2 * 3.14153f / N;
		// poz�ci�: XZ sugar� k�rvonal ment�n, Y = -0.25 magass�gban
		m_vb_cone_bottom.AddData(0, 0.25 * cosf(-a), -0.25, 0.25 * sinf(-a));
		// norm�lvektorok (b�r itt is mindegy)
		m_vb_cone_bottom.AddData(1, 0, -1, 0);
		// text�r�z�s: a text�ra k�zep�b�l kiv�gunk egy k�rlapot
		m_vb_cone_bottom.AddData(2, 0.5*cosf(a) + 0.5, 0.5*sinf(a) + 0.5);
	}

	// indices
	for (int i = 0; i < N; ++i){
		m_vb_cone_bottom.AddIndex(0, i + 2, i + 1);
	}

	m_vb_cone_bottom.InitBuffers();


	//
	// shaderek bet�lt�se
	//
	m_program.AttachShader(GL_VERTEX_SHADER, "dirLight.vert");
	m_program.AttachShader(GL_FRAGMENT_SHADER, "dirLight.frag");

	m_program.BindAttribLoc(0, "vs_in_pos");
	m_program.BindAttribLoc(1, "vs_in_normal");
	m_program.BindAttribLoc(2, "vs_in_tex0");

	if ( !m_program.LinkProgram() )
	{
		return false;
	}

	//
	// egy�b inicializ�l�s
	//

	m_camera.SetProj(45.0f, 640.0f/480.0f, 0.01f, 1000.0f);

	// text�ra bet�lt�se
	m_textureID = TextureFromFile("texture.png");

	// mesh bet�lt�s
	m_mesh = ObjParser::parse("Stargate.obj");
	m_mesh->initBuffers();

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_textureID);

	m_program.Clean();
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time)/1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// **********************************
	// A MEGVIL�G�T�S BE�LL�T�SAI
	// **********************************

	m_program.On();

	// akarunk-e f�nyeket
	m_program.SetUniform("light_switch", light_switch);

	// milyen ir�nyba �lljon a spotl�mpa? A lightDir-t homogeniz�ljuk,
	// majd u �s v szerinti forgat�st alkalmazunk rajta
	// (Z �s X tengelyek ment�n mennyivel forgassunk)
	glm::vec4 lightDirCurrent =
		glm::rotate<float>(v, 1, 0, 0) *
		glm::rotate<float>(u, 0, 0, 1) *
		glm::vec4(lightDir, 0);

	// �tadjuk a shadernek a poz�ci�t �s az ir�nyt
	m_program.SetUniform("light_pos", light_x, light_y, light_z);
	m_program.SetUniform("lightDir", lightDirCurrent.x, lightDirCurrent.y, lightDirCurrent.z);
	m_program.SetUniform("is_sharp", is_sharp);

	m_program.Off();

	// **********************************
	// K�P RAJZOL�S
	// **********************************

	m_program.On();

	// a k�pot ugyan�gy forgatjuk, ahogy a spotl�mpa ir�ny�t, �s ugyanoda is toljuk el
	glm::mat4 matWorld =
		glm::translate<float>(light_x, light_y, light_z) *
		glm::rotate<float>(v, 1, 0, 0) *
		glm::rotate<float>(u, 0, 0, 1);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);
	m_program.SetUniform("eye_pos", m_camera.GetEye());

	m_program.SetTexture("texImage", 0, m_textureID);

	// �rtes�tj�k a shader-t, hogy a k�pra ne sz�moljon f�nyeket (l�sd shader k�d)
	m_program.SetUniform("this_is_cone", true);
	// a k�p k�t r�szb�l tev�dik �ssze: pal�st...
	m_vb_cone.On();
	m_vb_cone.DrawIndexed(GL_TRIANGLES, 0, 3 * N, 0);
	m_vb_cone.Off();
	// ...�s als� lap
	m_vb_cone_bottom.On();
	m_vb_cone_bottom.DrawIndexed(GL_TRIANGLES, 0, 3 * N, 0);
	m_vb_cone_bottom.Off();
	m_program.SetUniform("this_is_cone", false);
	// ezut�n a r�sz ut�n ism�t sz�moljon f�nyeket

	// shader kikapcsolasa
	m_program.Off();

	// **********************************
	// TALAJ RAJZOL�S
	// **********************************

	m_program.On();

	matWorld = glm::mat4(1.0f);
	matWorldIT = glm::transpose( glm::inverse( matWorld ) );
	mvp = m_camera.GetViewProj() *matWorld;

	// a talajnak m�s anyagjellemz�ket szeretn�nk, mint a mesh eset�ben,
	// ez�rt a shaderben �t�rjuk a megfelel� uniform v�ltoz�kat
	m_program.SetUniform("Ka", 1, 1, 1, 1);
	m_program.SetUniform("Kd", 0.3, 1, 0.3, 1);
	m_program.SetUniform("Ks", 0, 0.5, 1, 1);

	m_program.SetUniform( "world", matWorld );
	m_program.SetUniform( "worldIT", matWorldIT );
	m_program.SetUniform( "MVP", mvp );
	m_program.SetUniform( "eye_pos", m_camera.GetEye() );

	m_program.SetTexture("texImage", 0, m_textureID);

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	m_vb_ground.On();
	m_vb_ground.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	m_vb_ground.Off();

	// shader kikapcsolasa
	m_program.Off();

	// **********************************
	// MESH RAJZOL�S
	// **********************************

	m_program.On();

	matWorld =
		glm::translate<float>(0, 0, 3) *
		glm::scale<float>(0.1, 0.1, 0.1);
	matWorldIT = glm::transpose( glm::inverse( matWorld ) );
	mvp = m_camera.GetViewProj() *matWorld;

	// a mesh kirajzol�s�n�l m�s anyagjellemz�ket szeretn�nk
	m_program.SetUniform("Ka", 1, 1, 1, 1);
	m_program.SetUniform("Kd", 1, 0.2, 0.2, 1);
	m_program.SetUniform("Ks", 1, 0.5, 1, 1);

	m_program.SetUniform( "world", matWorld );
	m_program.SetUniform( "worldIT", matWorldIT );
	m_program.SetUniform( "MVP", mvp );
	m_program.SetUniform( "eye_pos", m_camera.GetEye() );

	m_program.SetTexture("texImage", 0, m_textureID);

	m_mesh->draw();

	m_program.Off();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);

	switch (key.keysym.sym){
	case SDLK_q:
		is_sharp = !is_sharp;
		break;
	case SDLK_UP:
		light_z -= 0.5f;
		break;
	case SDLK_DOWN:
		light_z += 0.5f;
		break;
	case SDLK_LEFT:
		light_x -= 0.5f;
		break;
	case SDLK_RIGHT:
		light_x += 0.5f;
		break;
	case SDLK_f:
		light_y -= 0.5f;
		break;
	case SDLK_r:
		light_y += 0.5f;
		break;
	case SDLK_j:
		u -= 1.0f;
		break;
	case SDLK_l:
		u += 1.0f;
		break;
	case SDLK_i:
		v += 1.0f;
		break;
	case SDLK_k:
		v -= 1.0f;
		break;
	}
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
	if (mouse.button == SDL_BUTTON_RIGHT){
		light_switch = !light_switch;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}