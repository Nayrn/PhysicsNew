#pragma once
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"

class RigidBody 
{
public:
	glm::vec3 m_position;
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass);
	RigidBody();
	~RigidBody();
	glm::vec3 velocity;
	glm::vec4 colour;
	glm::vec3 acceleration;
	float m_mass;
	float elasticity;
	float drag;
	int userInt;
	float rotation2D; // only need a single float to represent rotation
	void update(glm::vec3 gravity, float timeStep); 
	virtual void debug();
	void applyForce(glm::vec3 force);
	void applyForcetoActor(RigidBody* actor2, glm::vec3 force);
	bool isKinematic;
	bool dynamicObj;

};
