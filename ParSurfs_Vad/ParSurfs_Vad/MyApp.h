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

// Utils
#include "gCamera.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

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

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// shaderekhez szükséges változók
	GLuint m_programID = 0; // shaderek programja

	// OpenGL-es dolgok

	// gomb geometria
	GLuint  m_vaoID = 0; // vertex array object erőforrás azonosító
	GLuint  m_vboID = 0; // vertex buffer object erőforrás azonosító
	GLuint  m_iboID = 0; // index buffer object erőforrás azonosító
	GLsizei m_index_count = 0; // mennyi indexet kell rajzolnunk

	// textura
	GLuint m_TextureID = 0; // betöltött textúra erőforrás azonosító
	
	// transzformációs mátrixok
	glm::mat4 m_matWorld = glm::mat4(1); // 4*4-es egységmátrix
	glm::mat4 m_matView = glm::mat4(1);
	glm::mat4 m_matProj = glm::mat4(1);

	// Feny tulajdonsagok
	glm::vec4 m_lightPos = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	glm::vec3 m_lightAmbient = glm::vec3(0.2f);
	glm::vec3 m_lightDiffuse = glm::vec3(1.0f);
	glm::vec3 m_lightSpecular = glm::vec3(1.0f);

	float m_lightConstantAttenuation = 1.0;
	float m_lightLinearAttenuation = 0.0;
	float m_lightQuadraticAttenuation = 0.0;

	// Kamera
	gCamera m_camera;

	// mátrixok helye a shaderekben
	GLuint	m_loc_world   = 0;
	GLuint	m_loc_worldIT = 0;
	GLuint	m_loc_view    = 0;
	GLuint	m_loc_proj    = 0;

	// textura helye a shaderben
	GLuint  m_loc_tex  = 0;

	// fenyforras parameterek helyei a shaderben

	GLuint	m_loc_light_pos = 0;

	GLuint	m_loc_La = 0;
	GLuint	m_loc_Ld = 0;
	GLuint	m_loc_Ls = 0;

	GLuint  m_loc_lightConstantAttenuation = 0;
	GLuint  m_loc_lightLinearAttenuation = 0;
	GLuint  m_loc_lightQuadraticAttenuation = 0;

	// anyag parameterek helyei a shaderben

	GLuint m_loc_Shininess = 0;

	// kamera poziciojanak helye a shaderben

	GLuint m_loc_camera_pos = 0;

	void InitParamSurfGeometry( );

	void InitShaders();
	void InitTextures();

	bool polygonModeWireframe = false; // poligon mod, lehet wireframere valtani
};

