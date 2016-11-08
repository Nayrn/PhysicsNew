#include "Plane.h"

void Plane::makeGizmo()
{
	// change vec2 to vec3 for collisions
	float lineSegLength = 300.0f;
	glm::vec3 centrePoint = m_vNormal * m_fDistance;
	glm::vec3 parallel = glm::cross(m_vNormal, glm::vec3(-1, 0, 0));
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec3 start = centrePoint + (parallel * lineSegLength);
	glm::vec3 end = centrePoint - (parallel * lineSegLength);
	//Gizmos::add2DLine(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y), colour);

	
//	glm::vec4 white(1);
//	glm::vec4 black(0, 0, 0, 1);

	for (int i = -300; i <= 300; i += 10)
	{
		Gizmos::addLine(glm::vec3(start.x + i, start.y, start.z), glm::vec3(end.x + i, end.y, end.z),
			i == 10 ? colour : colour);
		Gizmos::addLine(glm::vec3(start.x, start.y, start.z + i), glm::vec3(end.x, end.y, end.z + i),
			i == 10 ? colour : colour);
	}


}

Plane::Plane(glm::vec3 normal, float distance)
{
	m_vNormal = normal;
	m_fDistance = distance;
	m_shapeID = PLANE;
	
	dynamicObj = false;
	
}

Plane::Plane()
{
}

Plane::~Plane()
{
}
