#pragma once
#include "RigidBody.h"

class Plane : public RigidBody
{
public:
	glm::vec3 m_vNormal;
	float m_fDistance;
	void virtual makeGizmo();
	Plane(glm::vec3 normal, float distance);
	Plane();
	~Plane();

private:
protected:

};
