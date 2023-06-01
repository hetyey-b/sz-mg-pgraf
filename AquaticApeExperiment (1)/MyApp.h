#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Utils
#include "gCamera.h"

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

// mesh
#include "ObjParser_OGL3.h"

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
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_pointProgram;	  
	ProgramObject		m_axesProgram;
	ProgramObject		m_paramSurfProgram;

	// shield
	VertexArrayObject	m_ShieldVao;			// VAO
	IndexBuffer			m_ShieldIndices;		// index buffer
	ArrayBuffer			m_ShieldVertexBuffer;	// VBO
		
	// parametric surface
	VertexArrayObject	m_ParamSurfVao;			 // VAO
	IndexBuffer			m_ParamSurfIndices;		 // index buffer
	ArrayBuffer			m_ParamSurfVertexBuffer; // VBO
	GLuint				m_ParamSurfIndexNum = 0;

	// Kamera
	gCamera m_camera;

	Texture2D			m_shieldTexture;
	Texture2D			m_suzanneTexture;
	Texture2D			m_waterTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	std::unique_ptr<Mesh> m_mesh;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitShield();
	void InitParamSurface();

	// scene
	static const int MAX_POINT_COUNT = 10;

	float m_ElapsedTime = 0.0;
	float m_currentParam = 0.0;
	std::vector<glm::vec3> m_controlPoints;

	glm::vec3 EvaluatePathPosition(const float t) const;
	glm::vec3 EvaluatePathTangent(const float t) const;
};

