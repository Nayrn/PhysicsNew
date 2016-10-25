#pragma once
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	float _radius;
	Sphere(glm::vec3 position, glm::vec3 velocity,  float a_mass, float radius, glm::vec4 colour);
	glm::vec4 m_colour;
	void virtual update(glm::vec3 gravity, float timeStep) {};
	void virtual debug() {};
	glm::vec3 acceleration;
	virtual void makeGizmo();
	~Sphere();
	Sphere();

};