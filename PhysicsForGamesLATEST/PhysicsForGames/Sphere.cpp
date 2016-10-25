#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float a_mass, float radius, glm::vec4 colour)
{
	// GET RID OF EVERYTHING RB, IT BROKE EVERYTHING
	
	_radius = radius;
	m_position = position;
	m_colour = colour;
	acceleration = velocity / glfwGetTime();
	makeGizmo();   		
	m_massPO = a_mass;
	m_shapeID = SPHERE;
	hasRB = true;
	dynamicObj = true;
	isKinematic = false;
}


void Sphere::makeGizmo()
{
	Gizmos::addSphere(m_position, _radius, 10, 10, m_colour);	
}

Sphere::~Sphere()
{
	delete this;
}

Sphere::Sphere()
{
}
