#include "DIYPhysicsScene.h"
#include <algorithm>
// issue with deltaTime
DIYPhysicsScene::DIYPhysicsScene()
{
	// may have to make an array of spheres and iterate through for projectiles
	m_width = 1280;
	m_height = 720;
	m_AR = m_width / m_height;
	m_numObjects = actors.size();
	//spheres
	m_pObj = new Sphere(glm::vec3(0, 50, 0), glm::vec3(0, 0, 0), 0.1f, 0.5f, glm::vec4(0, 1, 0, 1));  
	newBall = new Sphere(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), 0.5f, 1.0f, glm::vec4(0, 0, 1, 1));
	m_pObj->elasticity = 0.5f;
	newBall->elasticity = 0.5f;

	//boxes
	boxOne = new Box(glm::vec3(100, 0, 60), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec4(1, 0, 0, 1), 0.5f);
	boxOne->elasticity = 0.5f;
	boxTwo = new Box(glm::vec3(40, 0, 60), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec4(0, 0, 1, 1), 1.1f);
	boxTwo->elasticity = 0.5f;
	//springs
	//springBall = new Sphere;
	//springBall1 = new Sphere;
	
	newPlane = new Plane(glm::vec3(0, 1, 0.), 1.0f);
	newPlane->m_massPO = 1000.0f;
	//springBall = new Sphere(glm::vec3(30, 30, 10), glm::vec3(0, 0, 0), 0.4, 1.0f, glm::vec4(0, 0, 0, 1));
	//springBall->drag = 0;
	//springBall->elasticity = 0.9f;
	//springBall->dynamicObj = false;
	//springBall->isKinematic = true;
	int numberBalls = 5;
	//for (int i = 1; i < numberBalls; i++)
	//{  
	//	springBall1 = new Sphere(glm::vec3(30 - (i * 3), 25 - (i * 3), 10), glm::vec3(0, 0, 0), 0.4, 1.0f, glm::vec4(0, 0, 0, 1));
	//	springBall1->drag = 0.0f;
	//	springBall1->elasticity = 0.1f;
	//	springBall1->userInt = 3;
	//	AddActor(springBall1);
	//	spring = new SpringJoint(springBall, springBall1, 0.05f, .999f);
	//	AddActor(spring);
	//	springBall1->dynamicObj = true;
	//}
}

void DIYPhysicsScene::AddActor(PhysicsObject * obj)
{
	actors.push_back(obj);	
	std::cout << actors.size() << std::endl;
}

void DIYPhysicsScene::removeActor(PhysicsObject * obj)
{
	actors.pop_back();	
}

void DIYPhysicsScene::update(float deltaTime)
{		

	////Iterate through each shape in the physics scene
	// makeGizmo()
	for (auto it = actors.begin(); it != actors.end(); it++)
	{
		(*it)->update(gravity, deltaTime);
	}
	
		checkForCol();

		for (auto it = actors.begin(); it != actors.end(); it++)
		{
			(*it)->makeGizmo();
		}

		

}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
//function pointer array for doing collisions 
static fn collisionfunctionArray[] =
{
	DIYPhysicsScene::plane2Plane,	DIYPhysicsScene::plane2Sphere, DIYPhysicsScene::plane2Box,
	DIYPhysicsScene::sphere2Plane,  DIYPhysicsScene::sphere2Sphere, DIYPhysicsScene::sphere2Box,
	DIYPhysicsScene::box2Plane,  DIYPhysicsScene::box2Sphere, DIYPhysicsScene::box2Box,

};

bool DIYPhysicsScene::checkForCol()
{
	// something wrong in this function
	int actorCount = actors.size();
	bool wasCollision = false;
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1 ; inner < actorCount; inner++)
		{

			PhysicsObject* obj1 = actors[outer];
			PhysicsObject* obj2 = actors[inner];
			if (obj1 == obj2)
				continue;
			else
			{
				int _shapeID1 = obj1->m_shapeID;
				int _shapeID2 = obj2->m_shapeID;
				if (_shapeID1 >= NUMBERSHAPE || _shapeID2 >= NUMBERSHAPE) continue;

				int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2;
				fn colFunPtr = collisionfunctionArray[functionIndex];

				if (colFunPtr != NULL)
				{
					wasCollision = colFunPtr(obj1, obj2) || wasCollision;
				}
			}

		}
	}
	return wasCollision;
}

void DIYPhysicsScene::calcRatioAndSeperate(PhysicsObject* obj1, PhysicsObject* obj2, glm::vec3 normal, float distance)
{
	//--CALCULATING RATIO HERE--
	// this function needs the seperate part -- need to access velocity
	RigidBody* rb1 = dynamic_cast<RigidBody*>(obj1);
	RigidBody* rb2 = dynamic_cast<RigidBody*>(obj2);

	float dist = distance;
	glm::vec3 norm = normal;

	float totalMass = rb1->m_massPO + obj2->m_massPO;
	float ob1Rat = rb2->m_massPO / totalMass;
	float ob2Rat = rb1->m_massPO / totalMass;
	// use timeStep instead

	rb1->m_position = rb1->m_position + ob1Rat *  dist * norm;
	rb2->m_position = rb2->m_position - ob2Rat * dist * norm;

	float avRest = 1.0f;
	float MassA = rb1->m_massPO;
	float MassB = rb2->m_massPO;
	glm::vec3 relVel = glm::vec3(0);
	
	//--SEPARATE OBJECTS HERE--
	// will return ratio from 0.0f to 1.0f, separate based on that
	// j = (1 + elasticity) vel^A-B . n      j is impulse, literally the reaction
	//		----------------------------
	//		1 / MassA + 1 / MassB

	relVel = (rb1->velocity - rb2->velocity);
	float impulse;
	float jTop = glm::dot(-(1 + rb1->elasticity) * relVel, normal);
	float jBottom = ((1 / MassA) + (1 / MassB));
	impulse = jTop / jBottom;
	// checking ratio, if other obj has higher ratio, give velocity

	rb1->velocity = rb1->velocity + (impulse / rb1->m_massPO * (normal));
	rb2->velocity = rb2->velocity - (impulse / rb2->m_massPO *(normal));


	
}


bool DIYPhysicsScene::plane2Plane(PhysicsObject* ob1, PhysicsObject* ob2)
{
	// not needed
	return false;
}

bool DIYPhysicsScene::plane2Sphere(PhysicsObject* ob1, PhysicsObject* ob2)
{
	if (sphere2Plane(ob2, ob1))
		return true;
	else
		return false;
}

bool DIYPhysicsScene::plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	if(box2Plane(obj2, obj1))
		return true;
	else
		return false;
}

bool DIYPhysicsScene::sphere2Plane(PhysicsObject* ob1, PhysicsObject* ob2)
{
	Sphere *sp = dynamic_cast<Sphere*>(ob1);
	Plane *plane = dynamic_cast<Plane*>(ob2);
	// need to take in width / normal
	if (sp != NULL && plane != NULL)
	{
		glm::vec3 colNormal = plane->m_vNormal;
		float sphere2planeCol = glm::dot(sp->m_position, plane->m_vNormal) - plane->m_fDistance;
		
		
		float intersectCol = sp->_radius - sphere2planeCol;
		
		if (intersectCol > 0)
		{
			calcRatioAndSeperate(sp, plane, colNormal, intersectCol); // possibly reverse
			return true;
		}
		else
		{
			return false;
		}

		
	}
	else
		return false;
}





bool DIYPhysicsScene::sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere *sp1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sp2 = dynamic_cast<Sphere*>(obj2);

	if (sp1 != NULL && sp2 != NULL)
	{
		
		float sp2sp = glm::distance(sp1->m_position, sp2->m_position);
		float spRadius = sp1->_radius + sp2->_radius;
		float col = sp2sp - spRadius;

		

		if (col <= 0) //colliding
		{
			glm::vec3 normal = sp2->m_position - sp1->m_position;
			//normal = glm::normalize(normal);

			calcRatioAndSeperate(sp1, sp2, normal, col);

			// set colour to red if overlapping and set velocity to 0
			sp1->m_colour = glm::vec4(1, 0, 0, 1);
			return true;

		}
		else
		{
			return false;
		}
	}
	else return false;
}


bool DIYPhysicsScene::sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* sp = dynamic_cast<Sphere*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);
	glm::vec3 normal;
	glm::vec3 offSet = sp->m_position - box->m_position;

	if (std::abs(offSet.x) > 0)
		offSet.x = std::min(std::abs(offSet.x), box->extents.x) * (std::abs(offSet.x) / offSet.x);
	if (std::abs(offSet.y) > 0)
		offSet.y = std::min(std::abs(offSet.y), box->extents.x) * (std::abs(offSet.y) / offSet.y);
	if (std::abs(offSet.z) > 0)
		offSet.z = std::min(std::abs(offSet.z), box->extents.z) * (std::abs(offSet.z) / offSet.z);

	glm::vec3 boxCol = box->m_position + offSet;
	offSet = boxCol - sp->m_position;

	float dist = glm::length(offSet);
	float colDist = sp->_radius - dist;
	
	if (colDist > 0)
	{
		if (offSet.x > 0 || offSet.y > 0 || offSet.z > 0)
		{
			normal = glm::normalize(offSet);
		}


		calcRatioAndSeperate(box, sp, normal, dist);
		return true;
	}
			
	return false;
}



bool DIYPhysicsScene::box2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	if (sphere2Box(obj2, obj1))
		return true;
	else
	return false;
}

bool DIYPhysicsScene::box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	float dist = glm::distance(box1->m_position, box2->m_position);
	glm::vec3 norm = box1->extents - box2->extents;

	// extents is key to AABB max & min
	// -- Box1 -- //
	glm::vec3 MaxB1 = box1->m_position + box1->extents;
	glm::vec3 MinB1 = box1->m_position - box1->extents;

	// --Box 2 -- //
	glm::vec3 MaxB2 = box2->m_position + box2->extents;
	glm::vec3 MinB2 = box2->m_position - box2->extents;

	bool isCol = false;
	//------ This whole chunk is correct 
	if (MinB1.x <= MaxB2.x && MaxB1.x >= MinB2.x)
		isCol = true;
	else if (MinB1.y <= MaxB2.y && MaxB1.y >= MinB2.y)
		isCol = true;
	else if (MinB1.z <= MaxB2.z && MaxB1.z >= MinB2.z)
		isCol = true;
	else
		isCol = false;
	//--------

	// calculate offset and then 
	glm::vec3 minOffset = MaxB1 - MinB2;
	glm::vec3 maxOffset = MinB1 - MaxB2;
	// check which direction they're overlapping in and separate them from there

	if (minOffset.x < 0 || maxOffset.x > 0 ||
		minOffset.y < 0 || maxOffset.y > 0 ||
		minOffset.z < 0 || maxOffset.z > 0)
	{
		return false;
	}
	else   // (condition) ? (if true) : (if false)
	{
		glm::vec3 newOff = glm::vec3(std::abs(minOffset.x) < std::abs(maxOffset.x) ? minOffset.x : maxOffset.x, //x
									 std::abs(minOffset.y) < std::abs(maxOffset.y) ? minOffset.y : maxOffset.y, //y
									 std::abs(minOffset.z) < std::abs(maxOffset.z) ? minOffset.z : maxOffset.z);//z
		// something wrong with position

		glm::vec3 offSet2;
		if (std::abs(newOff.x) < std::abs(newOff.y) && std::abs(newOff.x) < std::abs(newOff.z))
			offSet2 = glm::vec3(newOff.x, 0, 0);
		else if (std::abs(newOff.y) < std::abs(newOff.z))
			offSet2 = glm::vec3(0, newOff.y, 0);
		else
			offSet2 = glm::vec3(0, 0, newOff.z);


		glm::vec3 normal = glm::vec3(0, 1, 0);
		if (glm::length(newOff) > 0)
			normal = glm::normalize(offSet2);

		float colDist = glm::length(offSet2);
		calcRatioAndSeperate(box1, box2, normal, colDist);
		return true;
	}																											  

}


bool DIYPhysicsScene::box2Plane(PhysicsObject * ob1, PhysicsObject * ob2)
{
	// never getting called
	Box* box = dynamic_cast<Box*>(ob1);
	Plane* plane = dynamic_cast<Plane*>(ob2);

	glm::vec3 colNormal = plane->m_vNormal;
	 
	// will need planes normal , min and max of boxes and offset from plane

	//-- boxes
	glm::vec3 MaxB1 = box->m_position + box->extents;
	glm::vec3 MinB1 = box->m_position - box->extents;

	float distMin = glm::dot(MinB1, plane->m_vNormal) - plane->m_fDistance;
	float distMax = glm::dot(MaxB1, plane->m_vNormal) - plane->m_fDistance;

	float distNew = (distMin < distMax) ? distMin : distMax;
	if (distNew < 0)
	{
		calcRatioAndSeperate(box, plane, colNormal, -distNew);
		return true;
	}


	return false;

}

void DIYPhysicsScene::updateGizmos()
{
	Gizmos::clear();
}

void DIYPhysicsScene::debugScene()
{
}

void DIYPhysicsScene::addGizmos()
{
	
}

void DIYPhysicsScene::setUp()
{
	Gizmos::create();
	// creating deltaTime
	float currTime, prevTime, deltaTime;
	currTime = glfwGetTime();
	prevTime = currTime - glfwGetTime();
	deltaTime = currTime;



	
	gravity = glm::vec3(0, -10, 0);
	timeStep = 1.0f / 60.0f;
	
	AddActor(m_pObj);
	AddActor(newBall);
	//AddActor(springBall);
	//AddActor(springBall1);
	//AddActor(spring);
	AddActor(boxOne);
	AddActor(boxTwo);	
	AddActor(newPlane);
}

void DIYPhysicsScene::OnUpdate(float deltaTime)
{
	updateGizmos();
	update(timeStep);
} 

void DIYPhysicsScene::shutDown()
{
	delete newBall;
	delete m_pObj;
	delete boxOne;
	delete boxTwo;
	delete newPlane;
	//delete spring;
	//delete springBall1;
	//delete springBall;
	//incase of leaks
}
