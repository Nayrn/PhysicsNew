#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass)
{
	acceleration = velocity / glfwGetTime();
	m_massPO = mass;	
}


RigidBody::RigidBody() : 
	elasticity(1.0f),
	drag(0.1f)
{	
}

RigidBody::~RigidBody()
{
	
}

void RigidBody::update(glm::vec3 gravity, float timeStep)
{
	if (dynamicObj == true)
	{
		velocity += acceleration * timeStep;
		m_position += velocity * timeStep;
	}
	
	if (isKinematic)
	{
		velocity = glm::vec3(0, 0, 0);	
		acceleration = glm::vec3(0, 0, 0);
	}

	acceleration = glm::vec3(0.0f);
}

void RigidBody::debug()
{
}

void RigidBody::applyForce(glm::vec3 force)
{			
	acceleration += force / m_massPO;

}

void RigidBody::applyForcetoActor(PhysicsObject * actor2, glm::vec3 force)
{
	acceleration += force * m_massPO;
}
