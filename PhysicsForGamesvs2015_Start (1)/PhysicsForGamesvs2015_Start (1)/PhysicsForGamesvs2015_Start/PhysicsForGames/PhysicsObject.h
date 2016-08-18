#pragma once
#include <iostream>
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
};

class PhysicsObject
{
public: 
	ShapeType m_shapeID;
	virtual void update(glm::vec3 gravity, float timeStep) = 0;
	virtual void  debug() = 0;
	virtual void  makeGizmo() = 0;
	virtual void  resetPosition() {};
	float m_deltaTime;
	
};
