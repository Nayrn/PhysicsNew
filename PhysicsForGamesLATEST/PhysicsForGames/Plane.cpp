#include "Plane.h"

void Plane::makeGizmo()
{
	float lineSegLength = 300.0f;
	glm::vec2 centrePoint = m_vNormal * m_fDistance;
	glm::vec2 parallel = glm::vec2(m_vNormal.y, -m_vNormal.x);
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centrePoint + (parallel * lineSegLength);
	glm::vec2 end = centrePoint - (parallel * lineSegLength);
	Gizmos::add2DLine(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y), colour);

//	glm::vec4 white(1);
//	glm::vec4 black(0, 0, 0, 1);

	//for (int i = 0; i <= 20; ++i)
	//{
	//	Gizmos::addLine(glm::vec3(-10 + i, -0.01, -10), glm::vec3(-10 + i, -0.01, 10),
	//		i == 10 ? colour : colour);
	//	Gizmos::addLine(glm::vec3(-10, -0.01, -10 + i), glm::vec3(10, -0.01, -10 + i),
	//		i == 10 ? colour : colour);
	//}
// IT'S THE FUCKING NORMAL YOU WANT

}

Plane::Plane(glm::vec2 normal, float distance)
{
	m_vNormal = normal;
	m_fDistance = distance;
	m_shapeID = PLANE;


}

Plane::Plane()
{
}

Plane::~Plane()
{
}
