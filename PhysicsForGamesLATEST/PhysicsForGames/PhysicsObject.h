#pragma once
#include <iostream>
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Shapes.h"


enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX,
	NUMBERSHAPE,
	JOINT,
};

class PhysicsObject
{
public: 
	glm::vec3 m_position;
	ShapeType m_shapeID;
	virtual void update(glm::vec3 gravity, float timeStep) = 0;
	virtual void  debug() = 0;
	virtual void  makeGizmo() = 0;
	virtual void  resetPosition(glm::vec3 a_position) {};
	//float m_deltaTime; // is never defined, probably causing the problem
	float m_massPO;
	virtual void applyForcetoActor(PhysicsObject* actor2, glm::vec3 force) {};
	bool hasRB;
	

};


