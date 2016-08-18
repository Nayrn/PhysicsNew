#include "DIYPhysicsScene.h"
// edit to include 2nd ball
DIYPhysicsScene::DIYPhysicsScene()
{
	m_width = 1280;
	m_height = 720;
	m_AR = m_width / m_height;
	m_numObjects = actors.size();
	m_pObj = new Sphere(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), 0.4f, 10.0f, glm::vec4(0, 1, 0, 1));  
	newBall = new Sphere(glm::vec3(60, 10, 10), glm::vec3(0, 0, 0), 0.4f, 10.0f, glm::vec4(0, 0, 1, 1)); 

	magnet = new Sphere(glm::vec3(10, 80, 10), glm::vec3(0, 0, 0), 10.0f, 20, glm::vec4(1, 0, 1, 1)); // leave this here for now, have gravity act on it

	

}

void DIYPhysicsScene::AddActor(Sphere * obj)
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
	
	
	if (glfwGetKey(m_window, 78) == GLFW_PRESS)
	{
		newBall->applyForce(gravity);

	}

	sphere2sphere(newBall, m_pObj);
	float sp2sp = glm::distance(m_pObj->m_position, newBall->m_position);
	
	
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

		std::cout << col << std::endl;

		if (col < -1) //colliding
		{
			// set colour to red if overlapping and set velocity to 0
			sp1->m_colour = glm::vec4(1, 0, 0, 1);
			sp2->m_colour = glm::vec4(1, 0, 0, 1);

			sp1->velocity = glm::vec3(0, 0, 0);
			//sp2->velocity = glm::vec3(0, 0, 0);
			m_pObj->applyForcetoActor(newBall, gravity);
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

	
	gravity = glm::vec3(-10, 0, 0);
	timeStep = .001f;

	
	
	
	
	AddActor(newBall);
	AddActor(m_pObj);
	AddActor(magnet);
	newBall->update(gravity, deltaTime);	
	OnUpdate(deltaTime);





}

void DIYPhysicsScene::OnUpdate(float deltaTime)
{
	updateGizmos();
	update(deltaTime);


}
