#include "Box.h"
#include "Gizmos.h"

Box::Box(glm::vec3 a_centre, glm::vec3 a_extents, glm::vec4 colour, float a_mass)
{
	centre = a_centre;
	extents = a_extents;
	m_mass = a_mass;
	m_colourBox = colour;
	m_shapeID = BOX;
	m_mass = m_massPO;
	makeGizmo();
}



void Box::makeGizmo()
{
	Gizmos::addAABBFilled(centre, extents, m_colourBox);
}


Box::~Box()
{
}

Box::Box()
{
}
