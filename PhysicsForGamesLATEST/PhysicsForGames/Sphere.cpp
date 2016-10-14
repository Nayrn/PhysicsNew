#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float a_mass, float radius, glm::vec4 colour)
{
	_radius = radius;
	m_position = position;
	m_colour = colour;
	acceleration = velocity / glfwGetTime();
	makeGizmo();   	
	m_mass = a_mass;
	m_massPO = m_mass;
	m_shapeID = SPHERE;
}


void Sphere::makeGizmo()
{
	Gizmos::addSphere(m_position, _radius, 10, 10, m_colour);	
}

Sphere::~Sphere()
{
}

Sphere::Sphere()
{
}
