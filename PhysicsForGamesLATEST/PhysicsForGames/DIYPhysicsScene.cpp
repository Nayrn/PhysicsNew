#include "DIYPhysicsScene.h"
// edit to include 2nd ball
DIYPhysicsScene::DIYPhysicsScene()
{

	m_width = 1280;
	m_height = 720;
	m_AR = m_width / m_height;
	m_numObjects = actors.size();
	m_pObj = new Sphere(glm::vec3(-10, 10, 0), glm::vec3(0, 0, 0), 0.1f, 1.0f, glm::vec4(0, 1, 0, 1));  
	newBall = new Sphere(glm::vec3(60, 10, 10), glm::vec3(0, 0, 0), 0.1f, 1.0f, glm::vec4(0, 0, 1, 1)); 

	

	springBall = new Sphere(glm::vec3(30, 30, 10), glm::vec3(0, 0, 0), 0.4, 1.0f, glm::vec4(0, 0, 0, 1));
	springBall1 = new Sphere(glm::vec3(30, 40, 10), glm::vec3(0, 0, 0), 0.4, 1.0f, glm::vec4(0, 0, 0, 1));
	spring = new SpringJoint(springBall, springBall1, 0.8f, 10.0f);

	newPlane = new Plane(glm::vec2(20.0f, 20.0f), 1.0f);

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

	Gizmos::draw(glm::ortho<float>(-100, 100, -100 / m_AR, 100 / m_AR, -1.0f, 1.0f));
	spring->draw(springBall->m_position, springBall1->m_position);
	
	if (glfwGetKey(m_window, 77))
	{
		newBall->applyForce(glm::vec3(10, 0, 0));
	}
	if (glfwGetKey(m_window, 78) == GLFW_PRESS)
	{
		newBall->applyForce(gravity);
		m_pObj->applyForce(gravity);

	}
	sphere2sphere(newBall, m_pObj);
	sphere2plane(m_pObj, newPlane);
	
	float sp2sp = glm::distance(m_pObj->m_position, newBall->m_position);
	//std::cout << m_pObj->velocity.x << std::endl;
	
}

typedef void(*fn)(PhysicsObject*, PhysicsObject*);
//function pointer array for doing our collisions
static fn collisionfunctionArray[] =
{
	DIYPhysicsScene::plane2plane,	DIYPhysicsScene::plane2sphere,
	DIYPhysicsScene::sphere2plane,
};

void DIYPhysicsScene::checkForCol()
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

			int functionIndex = (_shapeID1 * NUMBERSHAPE) + _shapeID2; //NUMBERSHAPE?? the fuck??
			fn colFunPtr = collisionfunctionArray[functionIndex];
			
			if (colFunPtr != NULL)
				colFunPtr(obj1, obj2);
		}
	}
}

void DIYPhysicsScene::plane2plane(PhysicsObject* ob1, PhysicsObject* ob2)
{

}

void DIYPhysicsScene::plane2sphere(PhysicsObject* ob1, PhysicsObject* ob2)
{
}

void DIYPhysicsScene::sphere2plane(PhysicsObject* ob1, PhysicsObject* ob2)
{
	Sphere *sp = dynamic_cast<Sphere*>(ob1);
	Plane *plane = dynamic_cast<Plane*>(ob2);
	// need to take in width / normal
	if (sp != NULL && plane != NULL)
	{
		glm::vec2 colNormal = plane->m_vNormal;
		float sphere2plane = glm::dot(sp->m_position, plane->m_vNormal) - plane->m_fDistance;
		// the thing is in the tutorial, you need to change plane's stuff to vec3
	}
}


bool DIYPhysicsScene::sphere2sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere *sp1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sp2 = dynamic_cast<Sphere*>(obj2);

	if (sp1 != NULL && sp2 != NULL)
	{
		
		float sp2sp = glm::distance(sp1->m_position, sp2->m_position);
		float spRadius = sp1->_radius + sp2->_radius;
		float col = sp2sp - spRadius;

		

		if (col < -1) //colliding
		{
			// set colour to red if overlapping and set velocity to 0
			sp1->m_colour = glm::vec4(1, 0, 0, 1);
			sp2->m_colour = glm::vec4(1, 0, 0, 1);

			sp1->velocity = glm::vec3(0, 0, 0);
			//sp2->velocity = glm::vec3(0, 0, 0);
			m_pObj->applyForcetoActor(newBall, gravity / m_pObj->mass);
			newBall->applyForcetoActor(m_pObj, gravity / newBall->mass);
			return true;

		}
	}
	else return false;
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
	newBall->update(gravity, deltaTime);	
	OnUpdate(deltaTime);
	




}

void DIYPhysicsScene::OnUpdate(float deltaTime)
{
	updateGizmos();
	update(deltaTime);
	spring->update(glm::vec3(0, 10, 0), deltaTime);

}
