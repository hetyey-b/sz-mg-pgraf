#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"
#include "gShaderProgram.h"
#include "gVertexBuffer.h"
#include "Mesh_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// bels� elj�r�sok
	GLuint GenTexture();

	// OpenGL-es dolgok
	GLuint m_textureID; // text�ra er�forr�s azonos�t�

	gCamera			m_camera;
	gShaderProgram	m_program;
	gVertexBuffer	m_vb_ground;
	gVertexBuffer	m_vb_cone;
	gVertexBuffer	m_vb_cone_bottom;

	Mesh			*m_mesh;

	int N = 10;

	// u �s v fejezi ki a spotl�mpa x tengely ill. z tengely k�r�li elforgatotts�g�t
	// kezdetben nincs elforgatva
	float u = 0;
	float v = 0;
	// a spotl�mpa ir�nya, kezdetben lefel� n�z
	glm::vec3 lightDir = glm::vec3(0, -1, 0);
	// a spotl�mpa poz�ci�ja, kezdetben a (0, 10, 0) pontban
	float light_x = 0;
	float light_y = 10;
	float light_z = 0;
	
	// a spotl�mpa ki/be kapcsolhat�
	bool light_switch = true;

	bool is_sharp = true; // a spotl�mpa f�nye "�les" legyen-e
};

