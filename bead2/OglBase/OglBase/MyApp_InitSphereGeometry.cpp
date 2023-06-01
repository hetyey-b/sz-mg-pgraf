#include "MyApp.h"
#include <vector>
#include <math.h>


//
// egy parametrikus felület (u,v) paraméterértékekhez tartozó pontjának
// kiszámítását végző függvény
//
glm::vec3 CMyApp::GetUV(float u, float v)
{
	// origó középpontú, r sugarú gömb parametrikus alakja: http://hu.wikipedia.org/wiki/G%C3%B6mb#Egyenletek 
	// figyeljünk:	matematikában sokszor a Z tengely mutat felfelé, de nálunk az Y, tehát a legtöbb képlethez képest nálunk
	//				az Y és Z koordináták felcserélve szerepelnek
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float r = 2;

	return glm::vec3(r * sin(v) * cos(u),
		r * cos(v),
		r * sin(v) * sin(u));
}
//
// egy parametrikus felület (u,v) paraméterértékekhez tartozó normálvektorának
// kiszámítását végző függvény
//
glm::vec3 CMyApp::GetNorm(float u, float v)
{
	// Képlettel
	u *= float(2 * M_PI);
	v *= float(M_PI);
	return glm::vec3(sin(v) * cos(u), cos(v), sin(v) * sin(u));

	// Numerikusan (nem kell ismerni a képletet, elég a pozícióét)
	/*
	glm::vec3 du = GetPos(u+0.01, v)-GetPos(u-0.01, v);
	glm::vec3 dv = GetPos(u, v+0.01)-GetPos(u, v-0.01);

	return glm::normalize(glm::cross(du, dv));*/
}

glm::vec2 CMyApp::GetTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}


void CMyApp::InitSphereGeometry()
{
	// NxM darab négyszöggel közelítjük a parametrikus felületünket => (N+1)x(M+1) pontban kell kiértékelni
	static const int N = 80;
	static const int M = 40;


	// NxM darab négyszöggel közelítjük a parametrikus felületünket => (N+1)x(M+1) pontban kell kiértékelni
	std::vector<Vertex> vert((N + 1) * (M + 1));
	for (int j = 0; j <= M; ++j)
	{
		for (int i = 0; i <= N; ++i)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			int index = i + j * (N + 1);
			vert[index].p = GetUV(u, v);
			vert[index].n = GetNorm(u, v);
			vert[index].t = GetTex(u, v);
		}
	}

	// indexpuffer adatai: NxM négyszög = 2xNxM háromszög = háromszöglista esetén 3x2xNxM index
	std::vector<GLushort> indices(3 * 2 * (N) * (M));
	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			// minden négyszögre csináljunk kettő háromszöget, amelyek a következő 
			// (i,j) indexeknél született (u_i, v_j) paraméterértékekhez tartozó
			// pontokat kötik össze:
			// 
			// (i,j+1) C-----D (i+1,j+1)
			//         |\    |				A = p(u_i, v_j)
			//         | \   |				B = p(u_{i+1}, v_j)
			//         |  \  |				C = p(u_i, v_{j+1})
			//         |   \ |				D = p(u_{i+1}, v_{j+1})
			//         |    \|
			//   (i,j) A-----B (i+1,j)
			//
			// - az (i,j)-hez tartózó 1D-s index a VBO-ban: i+j*(N+1)
			// - az (i,j)-hez tartózó 1D-s index az IB-ben: i*6+j*6*N
			//		(mert minden négyszöghöz 2db háromszög = 6 index tartozik)
			//
			int index = i * 6 + j * (6 * N);
			indices[index + 0] = (i)+(j) * (N + 1);
			indices[index + 1] = (i + 1) + (j) * (N + 1);
			indices[index + 2] = (i)+(j + 1) * (N + 1);
			indices[index + 3] = (i + 1) + (j) * (N + 1);
			indices[index + 4] = (i + 1) + (j + 1) * (N + 1);
			indices[index + 5] = (i)+(j + 1) * (N + 1);
		}
	}

	m_sphere_index_count = static_cast<GLsizei>( indices.size() );

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_sphere_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_sphere_vaoID);

	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_sphere_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_sphere_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		vert.size() * sizeof(Vertex),		// ennyi bájt nagyságban
		vert.data(),	// erről a rendszermemóriabeli címről olvasva
		GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat

	// index puffer létrehozása
	glGenBuffers(1, &m_sphere_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphere_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

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
		(void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2); // ez lesz majd a szín
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(glm::vec3)));

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}