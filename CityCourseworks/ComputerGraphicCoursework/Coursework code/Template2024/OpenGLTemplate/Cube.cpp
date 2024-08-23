#include "Cube.h"
	
CCube::CCube()
{}

CCube::~CCube()
{
	Release();
}

void CCube::Create(string filename)
{
	m_tTexture.Load(filename);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);
	m_VBO.Create();
	m_VBO.Bind();
	
	// Write the code to add interleaved point, texture coord, and normal of the cube
	glm::vec3 v0 = glm::vec3(-1, -1, -1);//p0
	glm::vec3 v1 = glm::vec3(1, -1, -1);//p1
	glm::vec3 v2 = glm::vec3(-1, 1, -1);//p2
	glm::vec3 v3 = glm::vec3(1, 1, -1);//p3
	glm::vec3 v4 = glm::vec3(-1, -1, 1);//p4
	glm::vec3 v5 = glm::vec3(1, -1, 1);//p5
	glm::vec3 v6 = glm::vec3(-1, 1, 1);//p6
	glm::vec3 v7 = glm::vec3(1, 1, 1);//p7
	

	glm::vec3 n = glm::vec3(0, 0, 1);//4576
	glm::vec3 n1 = glm::vec3(0, 0, -1);//0132
	glm::vec3 n2 = glm::vec3(1, 0, 0);//5173
	glm::vec3 n3 = glm::vec3(-1, 0, 0);//0426
	glm::vec3 n4 = glm::vec3(0, 1, 0);//6723
	glm::vec3 n5 = glm::vec3(0, -1, 0);//0145
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(1, 0);
	glm::vec2 t2 = glm::vec2(1, 1);
	glm::vec2 t3 = glm::vec2(0, 1);

	
	//face n
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));
	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n, sizeof(glm::vec3));

	
	//face n1
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));
	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));
	
	
	//face n2
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));
	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));

	//face n3
	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));

	//face n4
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));
	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));

	//face n5
	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t3, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));

	
	// Upload data to GPU
	m_VBO.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
	
}

void CCube::Render()
{
	glBindVertexArray(m_uiVAO);
	m_tTexture.Bind();

	// Call glDrawArrays to render the side
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
}

void CCube::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_VBO.Release();
}

std::vector<glm::vec3> CCube::GetVertices() const {
	std::vector<glm::vec3> vertices;

	// Define vertices in the same order as they are added to the VBO in the Create function
	glm::vec3 v0 = glm::vec3(-1, -1, -1); // p0
	glm::vec3 v1 = glm::vec3(1, -1, -1);  // p1
	glm::vec3 v2 = glm::vec3(-1, 1, -1);  // p2
	glm::vec3 v3 = glm::vec3(1, 1, -1);   // p3
	glm::vec3 v4 = glm::vec3(-1, -1, 1);  // p4
	glm::vec3 v5 = glm::vec3(1, -1, 1);   // p5
	glm::vec3 v6 = glm::vec3(-1, 1, 1);   // p6
	glm::vec3 v7 = glm::vec3(1, 1, 1);    // p7

	// Add vertices to the vector in the same order as they are added to the VBO
	vertices.push_back(v4); // face n
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);

	vertices.push_back(v1); // face n1
	vertices.push_back(v0);
	vertices.push_back(v3);
	vertices.push_back(v2);

	vertices.push_back(v5); // face n2
	vertices.push_back(v1);
	vertices.push_back(v7);
	vertices.push_back(v3);

	vertices.push_back(v0); // face n3
	vertices.push_back(v4);
	vertices.push_back(v2);
	vertices.push_back(v6);

	vertices.push_back(v6); // face n4
	vertices.push_back(v7);
	vertices.push_back(v2);
	vertices.push_back(v3);

	vertices.push_back(v0); // face n5
	vertices.push_back(v1);
	vertices.push_back(v4);
	vertices.push_back(v5);

	return vertices;
}

