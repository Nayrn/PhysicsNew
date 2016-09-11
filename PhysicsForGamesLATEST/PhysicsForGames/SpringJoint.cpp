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
}

void SpringJoint::update(glm::vec3 a_gravity, float timeStep)
{
	//hooke's law in here
	//f=-kX
	a_gravity.x = -(m_springCoefficient * m_restLength) * timeStep;
	a_gravity.y = -(m_springCoefficient * m_restLength) * timeStep;
	a_gravity.z = -(m_springCoefficient * m_restLength) * timeStep;
}

void SpringJoint::debug()
{
}

void SpringJoint::makeGizmo()
{
}

void SpringJoint::draw(glm::vec3 pos_a, glm::vec3 pos_b)
{
	m_connections[0]->m_position = pos_a;
	m_connections[1]->m_position = pos_b;
	Gizmos::addLine(m_connections[0]->m_position, m_connections[1]->m_position, glm::vec4(0, 1, 0, 1));
}



