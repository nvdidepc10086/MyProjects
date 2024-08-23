#pragma once

#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"

// Class for generating a unit cube
class CTetrahedron
{
public:
	CTetrahedron();
	~CTetrahedron();
	void Create(string filename);
	void Render();
	void Release();
	std::vector < glm::vec3> GetVertices() const;
private:
	GLuint m_uiVAO;
	CVertexBufferObject m_VBO;
	CTexture m_tTexture;
	void AddTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec2 t0, glm::vec2 t1, glm::vec2 t2, glm::vec3 normal);
};
