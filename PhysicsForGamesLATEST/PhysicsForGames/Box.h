#pragma once
#include "RigidBody.h"


class Box : public RigidBody
{
public:
	Box(glm::vec3 a_centre, glm::vec3 a_velocity,  glm::vec3 a_extents, glm::vec4 colour, float a_mass);
	glm::vec4 m_colourBox;
	glm::vec3 centre, extents; // centre = position & extents = size
	glm::vec3 acceleration;
	virtual void makeGizmo();
	~Box();
	Box();
};

