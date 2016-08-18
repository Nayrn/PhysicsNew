#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass);
	RigidBody();
	~RigidBody();
	glm::vec3 m_position;
	glm::vec3 velocity;
	glm::vec4 colour;
	glm::vec3 acceleration;
	float mass;
	float rotation2D; // only need a single float to represent rotation
	void update(glm::vec3 gravity, float timeStep) override; // timestep == deltaTime?
	virtual void debug();
	void applyForce(glm::vec3 force);
	void applyForcetoActor(RigidBody* actor2, glm::vec3 force);
	bool isKinematic;
};
