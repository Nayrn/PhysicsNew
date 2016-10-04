#pragma once
#include <iostream>
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include <vector>
#include "PhysicsObject.h"
#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Sphere.h"
#include "SpringJoint.h"
#include "Plane.h"

class DIYPhysicsScene 
{
public:

	DIYPhysicsScene();
	void AddActor(PhysicsObject * obj);

	float timeStep;
	std::vector<PhysicsObject*> actors;
	//void AddActor(PhysicsObject*);  
	void removeActor(PhysicsObject*);
	void updateGizmos();
	void debugScene();
	void addGizmos();
	void setUp();
	void OnUpdate(float deltaTime);
	void SetWindow(GLFWwindow* pWindow) { m_window = pWindow; }

	static bool plane2Plane(PhysicsObject* ob1, PhysicsObject* ob2);
	static bool plane2Sphere(PhysicsObject* ob1, PhysicsObject* ob2);
	static bool plane2Box(PhysicsObject * obj1, PhysicsObject * obj2);

	static bool sphere2Plane(PhysicsObject* ob1, PhysicsObject* ob2);
	static bool sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool sphere2Box(PhysicsObject * obj1, PhysicsObject * obj2);

	static bool box2Plane(PhysicsObject* ob1, PhysicsObject* ob2);
	static bool box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool box2Box(PhysicsObject * obj1, PhysicsObject * obj2);

private:
	void update(float deltaTime);
	
	void checkForCol();
	int m_width;
	int m_height;
	int m_AR;
	int m_numObjects;
	Sphere* m_pObj;
	Sphere* newBall;
	Sphere* magnet;
	GLFWwindow * m_window;
	glm::vec3 m_attractionForce;
	Sphere* springBall;
	Sphere* springBall1;
	SpringJoint * spring;
	std::vector<Sphere*> ballActor;
	Plane* newPlane;
};

static glm::vec3 gravity;