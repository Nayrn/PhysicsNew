#include "Box.h"
#include "Gizmos.h"

//(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass);

Box::Box(glm::vec3 a_centre, glm::vec3 a_extents, glm::vec4 colour, float a_mass)
	:RigidBody()
{
	centre = a_centre;
	m_position = a_centre;
	extents = a_extents;
	m_colourBox = colour;
	m_shapeID = BOX;
	m_massPO = a_mass;
	makeGizmo();
	hasRB = true;
	dynamicObj = true;
	//isKinematic = false; <- does nothing
	// now only deltaTime is fucked for box, but mass is now being set correctly

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
