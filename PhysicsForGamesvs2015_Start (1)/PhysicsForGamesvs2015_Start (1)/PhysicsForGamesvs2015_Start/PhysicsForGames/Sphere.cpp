#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere(glm::vec3 position, glm::vec3 velocity, float a_mass, float radius, glm::vec4 colour)
{
	_radius = radius;
	m_position = position;
	m_colour = colour;
	acceleration = velocity / glfwGetTime();
	makeGizmo();   	
	mass = a_mass;
}


void Sphere::makeGizmo()
{
	Gizmos::addSphere(m_position, _radius, 10, 10, m_colour);	
}
