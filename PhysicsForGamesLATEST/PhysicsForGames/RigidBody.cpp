#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass)
{
	acceleration = velocity / glfwGetTime();
	m_position = position;
}

RigidBody::RigidBody()
{
	
}

RigidBody::~RigidBody()
{
}

void RigidBody::update(glm::vec3 gravity, float timeStep)
{
	//timeStep = glfwGetTime();
	if (dynamicObj)
	{
		velocity += acceleration * timeStep;
		m_position += velocity * timeStep;
	}
	
	acceleration = glm::vec3(0.0f);
}

void RigidBody::debug()
{
}

void RigidBody::applyForce(glm::vec3 force)
{			
	acceleration += force / mass;
}

void RigidBody::applyForcetoActor(PhysicsObject * actor2, glm::vec3 force)
{
	acceleration += force * actor2->mass;
}
