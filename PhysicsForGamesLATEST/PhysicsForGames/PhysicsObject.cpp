#include "PhysicsObject.h"

void PhysicsObject::update(glm::vec3 gravity, float timeStep)
{
	gravity.x *= timeStep;
	gravity.y *= timeStep;
	gravity.z *= timeStep;



	//ShapeTypeOne shape = ShapeTypeOne::SPHERE;
	//switch (shape)
	//{
	//case PLANE:
	//	rb->dynamicObj = false;
	//	break;
	//
	//case BOX:
	//	rb->dynamicObj = true;
	//	break;
	//
	//case SPHERE:
	//	rb->dynamicObj = true;
	//	break;
	//
	//}

}

