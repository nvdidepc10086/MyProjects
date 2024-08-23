#include "Tetrahedron.h"

CTetrahedron::CTetrahedron() {}

CTetrahedron::~CTetrahedron() {
    Release();
}

void CTetrahedron::Create(string filename) {
    m_tTexture.Load(filename);
    m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenVertexArrays(1, &m_uiVAO);
    glBindVertexArray(m_uiVAO);
    m_VBO.Create();
    m_VBO.Bind();

    // Define vertices, texture coordinates, and normals for the tetrahedron sides
    glm::vec3 top = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 frontRight = glm::vec3(1.0f, -1.0f, -1.0f);
    glm::vec3 frontLeft = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 back = glm::vec3(0.0f, -1.0f, 1.0f);

    glm::vec2 topTexCoord = glm::vec2(0.5f, 1.0f);
    glm::vec2 frontRightTexCoord = glm::vec2(1.0f, 0.0f);
    glm::vec2 frontLeftTexCoord = glm::vec2(0.0f, 0.0f);
    glm::vec2 backTexCoord = glm::vec2(0.5f, 0.5f);

    // Define normals for each side
    glm::vec3 topNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 frontNormal = glm::normalize(glm::cross(frontRight - top, frontLeft - top));
    glm::vec3 rightNormal = glm::normalize(glm::cross(top - frontRight, back - frontRight));
    glm::vec3 leftNormal = glm::normalize(glm::cross(frontLeft - top, back - frontLeft));
    glm::vec3 backNormal = glm::normalize(glm::cross(frontRight - back, frontLeft - back));

    // Add vertex data to VBO for each side
    AddTriangle(top, frontRight, frontLeft, topTexCoord, frontRightTexCoord, frontLeftTexCoord, topNormal);
    AddTriangle(top, frontLeft, back, topTexCoord, frontLeftTexCoord, backTexCoord, frontNormal);
    AddTriangle(top, back, frontRight, topTexCoord, backTexCoord, frontRightTexCoord, backNormal);
    AddTriangle(frontLeft, frontRight, back, frontLeftTexCoord, frontRightTexCoord, backTexCoord, rightNormal);

    // Upload data to GPU
    m_VBO.UploadDataToGPU(GL_STATIC_DRAW);
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void CTetrahedron::Render() {
    glBindVertexArray(m_uiVAO);
    m_tTexture.Bind();

    // Call glDrawArrays to render the tetrahedron
    glDrawArrays(GL_TRIANGLES, 0, 12); // 12 vertices for 4 triangles
}

void CTetrahedron::Release() {
    m_tTexture.Release();
    glDeleteVertexArrays(1, &m_uiVAO);
    m_VBO.Release();
}

void CTetrahedron::AddTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec2 t0, glm::vec2 t1, glm::vec2 t2, glm::vec3 normal) {
    m_VBO.AddData(&v0, sizeof(glm::vec3));
    m_VBO.AddData(&t0, sizeof(glm::vec2));
    m_VBO.AddData(&normal, sizeof(glm::vec3));

    m_VBO.AddData(&v1, sizeof(glm::vec3));
    m_VBO.AddData(&t1, sizeof(glm::vec2));
    m_VBO.AddData(&normal, sizeof(glm::vec3));

    m_VBO.AddData(&v2, sizeof(glm::vec3));
    m_VBO.AddData(&t2, sizeof(glm::vec2));
    m_VBO.AddData(&normal, sizeof(glm::vec3));
}
