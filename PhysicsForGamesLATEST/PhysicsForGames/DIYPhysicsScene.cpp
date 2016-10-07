#include "DIYPhysicsScene.h"
// edit to include 2nd ball
DIYPhysicsScene::DIYPhysicsScene()
{

	m_width = 1280;
	m_height = 720;
	m_AR = m_width / m_height;
	m_numObjects = actors.size();
	m_pObj = new Sphere(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), 0.1f, 1.0f, glm::vec4(0, 1, 0, 1));  
	newBall = new Sphere(glm::vec3(40, 10, 10), glm::vec3(0, 0, 0), 0.1f, 1.0f, glm::vec4(0, 0, 1, 1)); 
	// -- box centre is position --
	boxOne = new Box(glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), glm::vec4(1, 0, 0, 1), 0.1f);
	boxTwo = new Box(glm::vec3(0, 30, 1), glm::vec3(1, 1, 1), glm::vec4(0, 0, 1, 1), 0.1f);
	

	springBall = new Sphere;
	springBall1 = new Sphere;
	
	newPlane = new Plane(glm::vec3(0, 1, 0.), 1.0f);

	springBall = new Sphere(glm::vec3(30, 30, 10), glm::vec3(0, 0, 0), 0.4, 1.0f, glm::vec4(0, 0, 0, 1));
	springBall->drag = 0;
	springBall->elasticity = 0.9f;
	springBall->dynamicObj = false;
	
	int numberBalls = 5;
	for (int i = 1; i < numberBalls; i++)
	{
		springBall1 = new Sphere(glm::vec3(30 - (i * 3), 25 - (i * 3), 10), glm::vec3(0, 0, 0), 0.4, 1.0f, glm::vec4(0, 0, 0, 1));
		springBall1->drag = 0.1f;
		springBall1->elasticity = 0.9f;
		springBall1->userInt = 3;
		AddActor(springBall1);
		spring = new SpringJoint(springBall, springBall1, 0.1f, .999f);
		AddActor(spring);
	}
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
	
	
	
	//Iterate through each shape in the physics scene
	// makeGizmo()
	for (auto it = actors.begin(); it != actors.end(); it++)
	{
		(*it)->makeGizmo();
		(*it)->update(gravity, deltaTime);
	}
	
	checkForCol();
	
	
	//sphere2Plane(m_pObj, newPlane);  -- Shouldn't need this but do for some reason??

	if (glfwGetKey(m_window, 77))
	{
		newBall->applyForce(glm::vec3(10, 0, 0));
	}
	if (glfwGetKey(m_window, 78) == GLFW_PRESS)
	{
		newBall->applyForce(gravity);
		m_pObj->applyForce(gravity);
	
	}

}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
//function pointer array for doing collisions -- THIS SUDDENLY DOESN'T WORK --- HECK ---- DOING ME A FRIGHT ---
static fn collisionfunctionArray[] =
{
	DIYPhysicsScene::plane2Plane,	DIYPhysicsScene::plane2Sphere, DIYPhysicsScene::plane2Box,
	DIYPhysicsScene::sphere2Plane,  DIYPhysicsScene::sphere2Sphere, DIYPhysicsScene::sphere2Box,
	DIYPhysicsScene::box2Plane,  DIYPhysicsScene::box2Sphere, DIYPhysicsScene::box2Box,

};

bool DIYPhysicsScene::checkForCol() //--- ALSO HECK---
{
	int actorCount = actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* obj1 = actors[outer];
			PhysicsObject* obj2 = actors[inner];
			int _shapeID1 = obj1->m_shapeID;
			int _shapeID2 = obj2->m_shapeID;
			if (_shapeID1 >= NUMBERSHAPE || _shapeID2 >= NUMBERSHAPE) continue;

			int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2; 
			fn colFunPtr = collisionfunctionArray[functionIndex];
			
			if (colFunPtr != NULL)
			{
				colFunPtr(obj1, obj2);
				return true;
			}

		}
	}
	return false;
}

void DIYPhysicsScene::calcRatioAndSeperate(PhysicsObject* obj1, PhysicsObject* obj2, glm::vec3 normal, float distance)
{
	float dist = distance;
	glm::vec3 norm = normal;
	float totalMass = obj1->m_mass + obj2->m_mass;
	float ob1Rat = obj2->m_mass / totalMass;
	float ob2Rat = obj1->m_mass / totalMass;


	obj1->m_position = obj1->m_position - ob1Rat *  dist * norm;
	obj2->m_position = obj2->m_position + ob2Rat * dist * norm;

}

bool DIYPhysicsScene::plane2Plane(PhysicsObject* ob1, PhysicsObject* ob2)
{
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
		
		if (sphere2planeCol < 0)
		{
			colNormal *= -1;
			sphere2planeCol *= -1;
		}
	

		float intersectCol = sp->_radius - sphere2planeCol;
		
		if (intersectCol > 0)
		{
			calcRatioAndSeperate(plane, sp, colNormal, intersectCol);

			sp->velocity = glm::vec3(0, 0, 0);
			sp->colour = glm::vec4(1, 0, 0, 1);
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
			normal = glm::normalize(normal);

			calcRatioAndSeperate(sp1, sp2, normal, col);

			// set colour to red if overlapping and set velocity to 0
			sp1->m_colour = glm::vec4(1, 0, 0, 1);
			sp2->m_colour = glm::vec4(1, 0, 0, 1);

			sp1->velocity = glm::vec3(0, 0, 0);
			//sp2->velocity = glm::vec3(0, 0, 0);

			obj1->applyForcetoActor(obj2, gravity / obj1->m_mass);
			obj2->applyForcetoActor(obj1, gravity / obj2->m_mass);
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
	return false;
}

bool DIYPhysicsScene::box2Plane(PhysicsObject * ob1, PhysicsObject * ob2)
{
	return false;
}

bool DIYPhysicsScene::box2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool DIYPhysicsScene::box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	// extents is key to AABB max & min
	// X max
	float maxXB1 = box1->centre.x + box1->extents.x; // go off this thing
	float maxXB2 = box2->extents.x;

	// Y max
	float maxYB1 = box1->extents.y;
	float maxYB2 = box2->extents.y;

	// Z max
	float maxZB1 = box1->extents.z;
	float maxZB2 = box2->extents.z;

	// --HOW TO HECKING FIND MINIMUM???--

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

	// do not create physics scene here, create in physics
	// creating deltaTime
	float currTime, prevTime, deltaTime;
	currTime = glfwGetTime();
	prevTime = currTime - glfwGetTime();
	deltaTime = currTime;

	
	gravity = glm::vec3(0, -10, 0);
	timeStep = .001f;

	
	
	AddActor(m_pObj);
	AddActor(newBall);
	AddActor(springBall);
	AddActor(springBall1);
	AddActor(spring);
	AddActor(newPlane);
	AddActor(boxOne);
	AddActor(boxTwo);
	//OnUpdate(deltaTime);
	




}

void DIYPhysicsScene::OnUpdate(float deltaTime)
{
	updateGizmos();
	update(deltaTime);
	
}
