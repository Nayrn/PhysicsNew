#pragma once
#include "PhysicsObject.h"


class Box : public PhysicsObject
{
public:
	void virtual update(glm::vec3 gravity, float timeStep) {};
	void virtual debug() {};
	Box(glm::vec3 a_centre, glm::vec3 a_extents, glm::vec4 colour, float a_mass);
	glm::vec4 m_colourBox;
	glm::vec3 centre, extents; // position & size respectively
	virtual void makeGizmo();
	~Box();
	Box();
};

