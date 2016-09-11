#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	glm::vec2 m_vNormal;
	float m_fDistance;
	void virtual update(glm::vec3 gravity, float timeStep) {};
	void virtual debug() {};
	void virtual makeGizmo();
	Plane(glm::vec2 normal, float distance);
	Plane();
	~Plane();
private:
protected:

};
