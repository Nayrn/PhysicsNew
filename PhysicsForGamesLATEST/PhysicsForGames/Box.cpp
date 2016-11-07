#include "Box.h"
#include "Gizmos.h"

Box::Box(glm::vec3 a_centre, glm::vec3 a_velocity, glm::vec3 a_extents, glm::vec4 colour, float a_mass)
	:RigidBody()
{
	centre = a_centre;
	m_position = a_centre;
	extents = a_extents;
	m_colourBox = colour;
	m_shapeID = BOX;
	m_massPO = a_mass;
	hasRB = true;
	dynamicObj = true;
	acceleration = a_velocity / glfwGetTime();
	isKinematic = false;
	
}



void Box::makeGizmo()
{
	Gizmos::addAABBFilled(centre, extents, m_colourBox);
}


Box::~Box()
{
	delete this;
}

Box::Box()
{
}
