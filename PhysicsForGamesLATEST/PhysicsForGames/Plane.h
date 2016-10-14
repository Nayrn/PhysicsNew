#pragma once
#include "RigidBody.h"

class Plane : public RigidBody
{
public:
	glm::vec3 m_vNormal;
	float m_fDistance;
	void virtual update(glm::vec3 gravity, float timeStep) {};
	void virtual debug() {};
	void virtual makeGizmo();
	Plane(glm::vec3 normal, float distance);
	Plane();
	~Plane();

private:
protected:

};
