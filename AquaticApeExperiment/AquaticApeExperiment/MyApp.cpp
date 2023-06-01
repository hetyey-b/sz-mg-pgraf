#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(20, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_mesh = nullptr;
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitShield()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex> vertices =
	{
		{ glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 0) },
		{ glm::vec3(+0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 0) },
		{ glm::vec3(-0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 1) },
		{ glm::vec3(+0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 1) }
	};
	
	std::vector<int> indices =
	{
		0,1,2,
		2,1,3
	};

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_ShieldVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következő BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_ShieldVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_ShieldVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_ShieldIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_ShieldVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_ShieldVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_ShieldVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)),     sizeof(Vertex)>, m_ShieldVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_ShieldVertexBuffer },
		},
		m_ShieldIndices
	);
}

static glm::vec3 GetPos(float u, float v)
{
	glm::vec3 pos = glm::vec3(-10.0, 0.0, 10.0) + glm::vec3( 20.0, 0.0, -20.0) * glm::vec3(u, 0.0, v);

	return pos;
}

static glm::vec3 GetNorm(float u, float v)
{
	glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
	glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

	return glm::normalize(glm::cross(du, dv));
}

static glm::vec2 GetTex(float u, float v)
{
	return glm::vec2(u, v);
}

void CMyApp::InitParamSurface()
{
	static constexpr int N = 50;
	static constexpr int M = 50;

	std::vector<Vertex> vertices((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vertices[i + j * (N + 1)].p = GetPos(u, v);
			vertices[i + j * (N + 1)].n = GetNorm(u, v);
			vertices[i + j * (N + 1)].t = GetTex(u, v);
		}

	// indexpuffer adatai: NxM négyszög = 2xNxM háromszög = háromszöglista esetén 3x2xNxM index
	std::vector<GLuint> indices(3 * 2 * (N) * (M));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			// minden négyszögre csináljunk kettő háromszöget, amelyek a következő 
			// (i,j) indexeknél született (u_i, v_i) paraméterértékekhez tartozó
			// pontokat kötik össze:
			//
			//	(i,j+1)
			//		  o-----o(i+1,j+1)
			//		  |\    |			a = p(u_i, v_i)
			//		  | \   |			b = p(u_{i+1}, v_i)
			//		  |  \  |			c = p(u_i, v_{i+1})
			//		  |   \ |			d = p(u_{i+1}, v_{i+1})
			//		  |	 \|
			//	(i,j) o-----o(i+1, j)
			//
			// - az (i,j)-hez tartózó 1D-s index a VBO-ban: i+j*(N+1)
			// - az (i,j)-hez tartózó 1D-s index az IB-ben: i*6+j*6*(N+1) 
			//		(mert minden négyszöghöz 2db háromszög = 6 index tartozik)
			//
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_ParamSurfIndexNum = static_cast<int>( indices.size() );

	m_ParamSurfVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_ParamSurfIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_ParamSurfVao.Init(
		{
		   { CreateAttribute<0,						// attribútum: 0
							 glm::vec3,			// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
							 0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
							 sizeof(Vertex)		// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
						  >, m_ParamSurfVertexBuffer },
		   { CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_ParamSurfVertexBuffer },
		   { CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_ParamSurfVertexBuffer },
		},
		m_ParamSurfIndices
	);
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_program.LinkProgram();

	m_paramSurfProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "water.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_paramSurfProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_paramSurfProgram.LinkProgram();

	m_pointProgram.AttachShaders(
		{
			{ GL_VERTEX_SHADER,   "pointDrawer.vert"},
			{ GL_FRAGMENT_SHADER, "pointDrawer.frag"}
		}
	);
	m_pointProgram.LinkProgram();

	m_axesProgram.Init(
		{
			{ GL_VERTEX_SHADER,   "axes.vert"},
			{ GL_FRAGMENT_SHADER, "axes.frag"}
		}
	); // Egyuttal linkel is
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(1.0f,1.0f,1.0f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	glLineWidth(4.0f); // a vonalprimitívek vastagsága: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
	// a raszterizált pontprimitívek mérete
	glPointSize(15.0f);

	InitShaders();
	InitShield();
	InitParamSurface();
	

	// egyéb textúrák betöltése
	m_shieldTexture.FromFile("assets/danger_glass.png");
	m_suzanneTexture.FromFile("assets/marron.jpg");
	m_waterTexture.FromFile("assets/water_texture.jpg");

	// mesh betöltése
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Suzanne.obj"));
	m_mesh->initBuffers();
	
	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);

	// kontrolpontok
	m_controlPoints.push_back(glm::vec3(-1.0f, 0.0, -1.0f));
	m_controlPoints.push_back(glm::vec3( 1.0f, 0.0,  1.0f));

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();

	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	m_ElapsedTime += delta_time;

	last_time = SDL_GetTicks();

	m_camera.Update(delta_time);
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	// tengelyek

	m_axesProgram.Use();
	m_axesProgram.SetUniform("MVP", viewProj );
	
	glDrawArrays(GL_LINES, 0, 6);

	m_axesProgram.Unuse();

	//Suzanne

	m_program.Use();

    glm::vec3 forward = EvaluatePathTangent(m_currentParam);
    glm::vec3 right = glm::normalize(
        glm::cross( forward, glm::vec3(0.0f,1.0f,0.0f) )
    );

    glm::vec3 up = glm::cross( right, forward );

    glm::mat4 rot(1.0f);
    rot[0]=glm::vec4(forward,0.0f);
    rot[1]=glm::vec4(up,0.0f);
    rot[2]=glm::vec4(right,0.0f);

    glm::mat4 suzanneWorld = glm::translate(
        EvaluatePathPosition(m_currentParam)
    ) * rot * glm::mat4(
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

	m_program.SetTexture("texImage", 0, m_suzanneTexture);
	m_program.SetUniform("MVP", viewProj * suzanneWorld);
	m_program.SetUniform("world", suzanneWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(suzanneWorld)));
	m_mesh->draw();

	m_program.Unuse();
	
    // water 

    glm::mat4 waterWorld(1.0f);

    m_paramSurfProgram.Use();

    m_paramSurfProgram.SetTexture("texImage",0,m_waterTexture);
	m_program.SetUniform("MVP", viewProj * waterWorld);
	m_program.SetUniform("world", waterWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(waterWorld)));

    m_ParamSurfVao.Bind();

    glDrawElements(GL_TRIANGLES,m_ParamSurfIndexNum,GL_UNSIGNED_INT,nullptr);

    m_ParamSurfVao.Unbind();

    m_paramSurfProgram.Unuse();

	// controlpontok

	m_pointProgram.Use();
	m_pointProgram.SetUniform("MVP", viewProj );
	m_pointProgram.SetUniform("points", m_controlPoints);
	m_pointProgram.SetUniform("color", glm::vec4(1, 0, 1, 1));

	glDrawArrays(GL_POINTS, 0, (GLsizei)m_controlPoints.size());

	// kontrollpontokat összekötő szakaszok
	m_pointProgram.SetUniform("color", glm::vec4(0.5, 0.5, 0.5, 1));
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)m_controlPoints.size());
	m_pointProgram.Unuse();

    //shield 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    m_program.Use();

    m_ShieldVao.Bind();

    glm::mat4 shieldWorld = 
        glm::translate(glm::vec3(0.0f, 0.0f, 22.0f)) *
        glm::scale(glm::vec3(12.0f));

    m_program.SetTexture("texImage", 0, m_shieldTexture);
    m_program.SetUniform("MVP", viewProj * shieldWorld);
    m_program.SetUniform("world", shieldWorld);
    m_program.SetUniform("worldIT", glm::inverse(
                glm::transpose(shieldWorld)
                ));
    
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);

    m_ShieldVao.Unbind();
    
    m_program.Unuse();
    glDisable(GL_BLEND);

	//ImGui Testwindow
	ImGui::ShowTestWindow();
	
	if (ImGui::Begin("Gyak11"))
	{
		static int currentItem = -1;
	
		ImGui::ListBoxHeader("List", 4);
		for (int i = 0; i < static_cast<const int>(m_controlPoints.size()); ++i)
		{
			if (ImGui::Selectable(std::to_string(i).c_str(), (i == currentItem)))
				if (i == currentItem) currentItem = -1;
				else currentItem = i;
		}
		ImGui::ListBoxFooter();
	
		if (ImGui::Button("Add") && m_controlPoints.size() < MAX_POINT_COUNT)
		{
			m_controlPoints.push_back({ 0,0,0 });
			currentItem = static_cast<const int>(m_controlPoints.size() - 1);
		}
	
		ImGui::SameLine();
	
		if (ImGui::Button("Delete") && m_controlPoints.size() > 0 && currentItem < m_controlPoints.size() && currentItem != -1 )
		{
			m_controlPoints.erase(m_controlPoints.begin() + currentItem);
	
			
			if (m_controlPoints.size() == 0)
				currentItem = -1;
			else
				if (currentItem >= static_cast<const int>(m_controlPoints.size()) )
					currentItem  = static_cast<const int>(m_controlPoints.size() - 1 );
		}
	
		if (m_controlPoints.size() > 0 && currentItem != -1 )
			ImGui::SliderFloat3("Coordinates", &(m_controlPoints[currentItem][0]), -10, 10);
	
		ImGui::SliderFloat("Parameter", &m_currentParam, 0, (float)(m_controlPoints.size() - 1));
	}
	ImGui::End();

}

glm::vec3 CMyApp::EvaluatePathPosition(const float t) const
{
	if (m_controlPoints.size() == 0)
		return glm::vec3(0);

	const int interval = (const int)t;

	if (interval < 0)
		return m_controlPoints[0];

	if (interval >= m_controlPoints.size() - 1)
		return m_controlPoints[m_controlPoints.size() - 1];

	float localT = t - interval;
	return (1 - localT) * m_controlPoints[interval] + localT * m_controlPoints[interval + 1];
}

glm::vec3 CMyApp::EvaluatePathTangent(const float t) const
{
	if (m_controlPoints.size() < 2)
		return glm::vec3(1.0,0.0,0.0);

	int interval = (int)t;

	if (interval < 0)
		interval = 0;

	if (interval >= m_controlPoints.size() - 1)
		interval = static_cast<int>( m_controlPoints.size() - 2 );

	return glm::normalize(m_controlPoints[interval + 1] - m_controlPoints[interval]);
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
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
