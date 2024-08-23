#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Camera.h"
#include "Cube.h"


CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints()
{

	// Set control points (m_controlPoints) here, or load from disk

	m_controlPoints.push_back(glm::vec3(20,1,0));
	m_controlPoints.push_back(glm::vec3(20,1,10));
	m_controlPoints.push_back(glm::vec3(15,1,20));
	m_controlPoints.push_back(glm::vec3(10,3,10));
	m_controlPoints.push_back(glm::vec3(0,5,20));
	m_controlPoints.push_back(glm::vec3(-10,1,10));
	m_controlPoints.push_back(glm::vec3(-15,1,20));
	m_controlPoints.push_back(glm::vec3(-20,1,10));
	m_controlPoints.push_back(glm::vec3(-20,1,0));
	m_controlPoints.push_back(glm::vec3(-15,1,-10));
	m_controlPoints.push_back(glm::vec3(-10,2,-15));
	m_controlPoints.push_back(glm::vec3(0,5,-20));
	m_controlPoints.push_back(glm::vec3(10,1,-15));
	m_controlPoints.push_back(glm::vec3(15,1,-10));
	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)

}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size() - 1; i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();
	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(5000);
	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); ++i) {
		glm::vec3 v = m_centrelinePoints[i];
		vbo.AddData(&v, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
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


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.

	 // Clear any existing offset points
	m_leftOffsetPoints.clear();
	m_rightOffsetPoints.clear();
	inner_curve.clear();
	outer_curve.clear();

	// Compute the width of the track (you need to define 'w' somewhere)
	float w = 4.0f; // Define the width of the track



	// Iterate over each centreline point
	for (size_t i = 0; i < m_centrelinePoints.size(); ++i) {
		// Determine the current point and the next point on the centreline
		glm::vec3 p = m_centrelinePoints[i];
		glm::vec3 pNext = m_centrelinePoints[(i + 1) % m_centrelinePoints.size()]; // Wrap around at the end

		// Compute the tangent vector
		glm::vec3 T = glm::normalize(pNext - p);

		// Compute the binormal vector (using the y-axis as the up vector)
		glm::vec3 B = glm::cross(glm::vec3(0, 0, 1), T);
		B = glm::normalize(B);

		// Compute the normal vector
		glm::vec3 N = glm::cross(T, B);
		N = glm::normalize(N);

		// Compute left and right offset points
		glm::vec3 leftOffset = p - (w / 2) * N;
		glm::vec3 rightOffset = p + (w / 2) * N;

		if (p.z > 0.001) {
			// Store the left and right offset points
			outer_curve.push_back(leftOffset);
			inner_curve.push_back(rightOffset);
		}
		else if (p.z < 0) {
			// Store the left and right offset points
			inner_curve.push_back(leftOffset);
			outer_curve.push_back(rightOffset);
		}
		
	}

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	glGenVertexArrays(1, &vao_innerCurve);
	glBindVertexArray(vao_innerCurve);

	// Create a VBO for left offset points
	CVertexBufferObject innerVBO;
	innerVBO.Create();
	innerVBO.Bind();

	for (unsigned int i = 0; i < inner_curve.size(); ++i) {
		glm::vec3 v = inner_curve[i];
		innerVBO.AddData(&v, sizeof(glm::vec3));
	}
	innerVBO.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// Generate another VAO and VBO for right offset points
	glGenVertexArrays(1, &vao_outerCurve);
	glBindVertexArray(vao_outerCurve);

	// Create a VBO for right offset points
	CVertexBufferObject OuterVBO;
	OuterVBO.Create();
	OuterVBO.Bind();

	for (unsigned int i = 0; i < outer_curve.size(); ++i) {
		glm::vec3 v = outer_curve[i];
		OuterVBO.AddData(&v, sizeof(glm::vec3));
	}
	OuterVBO.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}

void CCatmullRom::CreateTrack(string filename)
{
	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	// Add texture to Path
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	// Create a VBO for the track path
	CVertexBufferObject trackVBO;
	trackVBO.Create();
	trackVBO.Bind();

	// Determine the vertices for the track path
	std::vector<glm::vec3> trackVertices;
	for (size_t i = 0; i < inner_curve.size(); ++i) {
		// Alternate between left and right offset points
		trackVertices.push_back(inner_curve[i]);
		trackVertices.push_back(outer_curve[i]);
	}

	// Ensure the path loops back to the beginning
	trackVertices.push_back(inner_curve[0]);
	trackVertices.push_back(outer_curve[0]);

	// Store the vertices in the VBO
	for (const auto& vertex : trackVertices) {
		const glm::vec3* vertexPtr = &vertex;
		trackVBO.AddData(reinterpret_cast<void*>(const_cast<glm::vec3*>(vertexPtr)), sizeof(glm::vec3));
	}
	trackVBO.UploadDataToGPU(GL_STATIC_DRAW);

	// Update the vertex count
	m_vertexCount = trackVertices.size();

	// Set the vertex attribute locations
	GLsizei stride = sizeof(glm::vec3);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

}

/*
void CCatmullRom::CreateTrackUsingCubes(string textureFilename) {
	// Generate a VAO called m_vaoTrack and a VBO to store the track points on the graphics card
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	// Create a VBO for the track points
	CVertexBufferObject trackVBO;
	trackVBO.Create();
	trackVBO.Bind();

	// Determine the vertices for the track points using cubes along the centerline
	for (size_t i = 0; i < m_centrelinePoints.size(); ++i) {
		// Create a cube at each centerline point
		CCube cube;
		cube.Create(textureFilename);

		// Translate the cube to the current centerline point
		glm::vec3 currentPosition = m_centrelinePoints[i];
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), currentPosition);

		// Get the cube's vertices and store them in the VBO
		const std::vector<glm::vec3>& cubeVertices = cube.GetVertices();
		for (const auto& vertex : cubeVertices) {
			glm::vec4 transformedVertex = modelMatrix * glm::vec4(vertex, 1.0f);
			trackVBO.AddData(reinterpret_cast<void*>(&transformedVertex), sizeof(glm::vec4));
		}
	}

	// Upload data to GPU
	trackVBO.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = sizeof(glm::vec4);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Update the vertex count
	m_vertexCount = m_centrelinePoints.size() * 36; // Each cube has 36 vertices
}

void CCatmullRom::RenderTrackUsingCubes() {
	// Bind the VAO containing the track points
	glBindVertexArray(m_vaoTrack);

	// Render the track
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

	// Restore default VAO
	glBindVertexArray(0);
}
*/


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_LINE_STRIP, 0, m_centrelinePoints.size());

}

void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it
	glBindVertexArray(vao_innerCurve);
	glDrawArrays(GL_LINE_STRIP, 0, inner_curve.size());

	// Bind the VAO m_vaoRightOffsetCurve and render it
	glBindVertexArray(vao_outerCurve);
	glDrawArrays(GL_LINE_STRIP, 0, outer_curve.size());
}


void CCatmullRom::RenderTrack()
{
	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();
	// Render the track as wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);

	// Restore filled polygons for the rest of the scene
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}


//Model move along the center line
glm::vec3 CCatmullRom::MoveAlongCenterline(const glm::vec3& currentPosition, float distance) {
	// Find the closest point on the centerline to the current position
	float minDistance = std::numeric_limits<float>::max();
	glm::vec3 closestPoint;

	for (const auto& point : m_centrelinePoints) {
		float d = glm::distance(currentPosition, point);
		if (d < minDistance) {
			minDistance = d;
			closestPoint = point;
		}
	}

	// Find the next point on the centerline
	glm::vec3 nextPoint;
	auto iter = std::find(m_centrelinePoints.begin(), m_centrelinePoints.end(), closestPoint);
	if (iter != m_centrelinePoints.end() && std::distance(iter, m_centrelinePoints.end()) > 1) {
		nextPoint = *(iter + 1);
	}
	else {
		// Handle reaching the end of the centerline (loop back to the start)
		nextPoint = m_centrelinePoints.front();
	}

	// Move towards the next point along the centerline
	glm::vec3 direction = glm::normalize(nextPoint - currentPosition);
	glm::vec3 newPosition = currentPosition + direction * distance;

	// Calculate the new view direction to face the same direction as the movement
	glm::vec3 newView = newPosition + direction;

	// Compute the UZI model's orientation
	glm::vec3 T = glm::normalize(direction); // Tangent vector
	glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0))); // Normal vector
	glm::vec3 B = glm::normalize(glm::cross(N, T)); // Binormal vector

	// Create the orientation matrix
	glm::mat4 uziOrientation = glm::mat4(glm::mat3(T, B, N));

	return newPosition;
}

glm::mat4 CCatmullRom::ChangeOrientationOfModel(const glm::vec3& currentPosition, float distance) {
	// Find the closest point on the centerline to the current position
	float minDistance = std::numeric_limits<float>::max();
	glm::vec3 closestPoint;

	for (const auto& point : m_centrelinePoints) {
		float d = glm::distance(currentPosition, point);
		if (d < minDistance) {
			minDistance = d;
			closestPoint = point;
		}
	}

	// Find the next point on the centerline
	glm::vec3 nextPoint;
	auto iter = std::find(m_centrelinePoints.begin(), m_centrelinePoints.end(), closestPoint);
	if (iter != m_centrelinePoints.end() && std::distance(iter, m_centrelinePoints.end()) > 1) {
		nextPoint = *(iter + 1);
	}
	else {
		// Handle reaching the end of the centerline (loop back to the start)
		nextPoint = m_centrelinePoints.front();
	}

	// Move towards the next point along the centerline
	glm::vec3 direction = glm::normalize(nextPoint - currentPosition);
	glm::vec3 newPosition = currentPosition + direction * distance;

	// Calculate the new view direction to face the same direction as the movement
	glm::vec3 newView = newPosition + direction;

	// Compute the UZI model's orientation
	glm::vec3 T = glm::normalize(direction); // Tangent vector
	glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0))); // Normal vector
	glm::vec3 B = glm::normalize(glm::cross(N, T)); // Binormal vector

	// Create the orientation matrix
	glm::mat4 uziOrientation = glm::mat4(glm::mat3(T, B, N));

	// Apply a 90 degree rotation around the up vector (Y-axis)
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	uziOrientation = rotationMatrix * uziOrientation;

	return uziOrientation;
}


glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);