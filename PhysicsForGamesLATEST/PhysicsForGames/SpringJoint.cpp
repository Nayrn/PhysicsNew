#include "SpringJoint.h"
#include <iostream>
#include "Gizmos.h"
SpringJoint::SpringJoint(RigidBody * connection1, RigidBody * connection2, float springCoefficient, float damping)
{
	
	m_connections[0] = connection1;
	m_connections[1] = connection2;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_restLength = glm::distance(m_connections[0]->m_position, m_connections[1]->m_position);
	m_shapeID = JOINT;
}

SpringJoint::SpringJoint()
{
}


SpringJoint::~SpringJoint()
{
	delete this;
}

void SpringJoint::update(glm::vec3 a_gravity, float timeStep)
{
	//hooke's law in here
	//f=-kX -bv
	//a_gravity.x = -m_springCoefficient *(m_restLength) * timeStep;
	//a_gravity.y = -m_springCoefficient *(m_restLength) * timeStep;
	//a_gravity.z = -m_springCoefficient *(m_restLength) * timeStep;

	float force = -m_springCoefficient *(m_restLength);

	glm::vec3 dir = m_connections[0]->m_position - m_connections[1]->m_position;
	dir = glm::normalize(dir);

	m_connections[0]->applyForce(dir * force);
	m_connections[1]->applyForce(dir * -force);
}

void SpringJoint::debug()
{
}

void SpringJoint::makeGizmo()
{
	Gizmos::addLine(m_connections[0]->m_position, m_connections[1]->m_position, glm::vec4(0, 1, 0, 1));
}


