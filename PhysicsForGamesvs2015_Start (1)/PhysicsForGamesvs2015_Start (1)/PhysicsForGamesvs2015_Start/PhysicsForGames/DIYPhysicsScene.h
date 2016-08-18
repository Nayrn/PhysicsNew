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


class DIYPhysicsScene 
{
public:

	DIYPhysicsScene();
	void AddActor(Sphere * obj);
	glm::vec3 gravity;
	float timeStep;
	std::vector<PhysicsObject*> actors;
	//void AddActor(PhysicsObject*);  
	void removeActor(PhysicsObject*);
	void updateGizmos();
	void debugScene();
	void addGizmos();
	void setUp();
	void OnUpdate(float deltaTime);
	//DIYPhysicsScene* physicsScene;

	void SetWindow(GLFWwindow* pWindow) { m_window = pWindow; }

private:
	void update(float deltaTime);


	bool sphere2sphere(PhysicsObject* obj1, PhysicsObject* obj2); // make spheres turn red on collision
	int m_width;
	int m_height;
	int m_AR;
	int m_numObjects;
	Sphere* m_pObj;
	Sphere* newBall;
	Sphere* magnet;
	GLFWwindow * m_window;
	glm::vec3 m_attractionForce;


};