#include "MyApp.h"
#include <math.h>

float clearColorX = 0.125f;
float clearColorY = 0.25f;
float clearColorZ = 0.5f;

int width = 1;
int height = 1;

bool changeColor = false;

CMyApp::CMyApp(void)
{
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
    glClearColor(clearColorX, clearColorY, clearColorZ, 1.0f);
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // �ssze "vagyolt" flagek
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
    switch(key.keysym.sym) {
        case SDLK_LCTRL:
            changeColor = true;
            break;
    }
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
    switch(key.keysym.sym) {
        case SDLK_LCTRL:
            changeColor = false;
            break;
    }
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
    if (changeColor) {
        clearColorX = mouse.x / (float)width; 
        clearColorY = mouse.y / (float)height; 
        clearColorZ = (mouse.x + mouse.y) / ((float)height + float(width)); 
    }
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

// a k�t param�terben az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
    width = _w;
    height = _h;
	glViewport(0, 0, _w, _h );
}
