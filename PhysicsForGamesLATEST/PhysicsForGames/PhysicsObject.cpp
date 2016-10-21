#include "PhysicsObject.h"
#include "RigidBody.h"


void PhysicsObject::update(glm::vec3 gravity, float timeStep)
{
	gravity.x *= timeStep;
	gravity.y *= timeStep;
	gravity.z *= timeStep;

	

}

