#include "Box.h"
#include "Gizmos.h"

Box::Box(glm::vec3 a_centre, glm::vec3 a_velocity, glm::vec3 a_extents, glm::vec4 colour, float a_mass)
	:RigidBody()
{
	
	//m_position = a_centre;
	//centre = m_position;
	extents = a_extents;
	m_colourBox = colour;
	m_shapeID = BOX;
	m_massPO = a_mass;
	hasRB = true;
	dynamicObj = true;
	acceleration = a_velocity / glfwGetTime();
	isKinematic = false;
	//centre = a_centre;
	m_position = a_centre;

}



void Box::makeGizmo()
{
	Gizmos::addAABBFilled(m_position, extents, m_colourBox);
}


Box::~Box()
{
}

Box::Box()
{
}
